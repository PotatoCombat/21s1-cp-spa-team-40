#include "source_processor/parser/Lexer.h"
#include <algorithm>
#include <iostream>
#include <tuple>

// tokenize file input

vector<Line> Lexer::tokenizeFile(fstream &file) {
    vector<Line> programLines = {};
    vector<vector<string>> programTokens = mergeLine(file);
    int stmtNum = 0;
    vector<string> currString = {};
    for (int i = 0; i < programTokens.size(); i++) {
        vector<string> subString = programTokens[i];
        currString.insert(currString.end(), subString.begin(), subString.end());

        if (!isCurlyBracket(currString.back()) && !isSemiColon(currString.back())) {
            continue;
        }

        if (isStmt(currString)) {
            stmtNum++;
        }

        Line curr = Line(stmtNum, currString);
        programLines.push_back(curr);
        currString = {};
    }

    if (!brackets.empty()) {
        throw invalid_argument("invalid program, brackets do not match");
    }
    return programLines;
}

vector<vector<string>> Lexer::mergeLine(fstream &file) {
    string input;
    vector<vector<string>> programTokens = {};

    while (getline(file, input)) {
        vector<string> line = tokenizeLine(input);
        tuple<vector<string>, vector<string>> splitString = splitLine(line);
        vector<string> currString = get<0>(splitString);
        vector<string> nextString = get<1>(splitString);

        if (!currString.empty()) {
            programTokens.push_back(currString);
        }

        while (nextString.size() > 0) {
            splitString = splitLine(nextString);
            currString = get<0>(splitString);
            nextString = get<1>(splitString);

            if (!currString.empty()) {
                programTokens.push_back(currString);
            }
        }
    }
    return programTokens;
}

tuple<vector<string>, vector<string>> Lexer::splitLine(vector<string> line) {
    tuple<vector<string>, vector<string>> splitString;
    vector<string> currString = {};
    vector<string> nextString = {};

    for (int i = 0; i < line.size(); i++) {
        string curr = line[i];
        if ((isCurlyBracket(curr)) || isSemiColon(curr)) {
            currString.push_back(curr);
            nextString = vector<string>(line.begin() + i + 1, line.end());
            break;
        }
        currString.push_back(curr);
    }

    get<0>(splitString) = currString;
    get<1>(splitString) = nextString;
    return splitString;
}

vector<string> Lexer::tokenizeLine(string input) {
    vector<string> inputLine = {};
    string currString = "";
    for (int i = 0; i < input.size(); i++) {
        char curr = input.at(i);
        currString = cleanString(currString);

        if (isOperator(curr) || isBracket(curr) || isSemiColon(curr)) {
            // push back previous string before special character
            addString(currString, inputLine);
            currString.push_back(curr);
            tokenizeAndAddSymbol(input, i, curr, currString, inputLine);
            checkValidBracket(curr);

        } else {
            currString.push_back(curr);
            if (isKeyword(currString)) {
                tokenizeAndAddKeyword(input, i, currString, inputLine);
            }
        }
    }

    currString = cleanString(currString);
    if (!currString.empty()) {
        inputLine.push_back(currString);
    }
    return inputLine;
}

// helper functions

void Lexer::tokenizeAndAddSymbol(string input, int &i, char curr, string &currString,
                                 vector<string> &inputLine) {
    // check if operator has an additional character
    if (i < input.size() - 1) {
        char next = input.at(i + 1);
        if (isOperator(next) && isOperator(curr)) {
            currString.push_back(next);
            i++;
        }
    }
    addString(currString, inputLine);
}

void Lexer::tokenizeAndAddKeyword(string input, int &i, string &currString,
                                  vector<string> &inputLine) {
    // check if keyword has an additional character
    if (i < input.size() - 1) {
        if (!isalnum(input.at(i + 1))) {
            addString(currString, inputLine);
        }
    } else {
        addString(currString, inputLine);
    }
}

void Lexer::addString(string &input, vector<string> &inputVector) {
    input = cleanString(input);
    if (!input.empty()) {
        inputVector.push_back(input);
    }
    input = "";
}

string Lexer::cleanString(string input) {
    input.erase(remove_if(input.begin(), input.end(), ::isspace), input.end());
    return input;
}

void Lexer::checkValidBracket(char curr) {
    if (isBracket(curr)) {
        if (curr == '(' || curr == '{') {
            brackets.push(curr);
        }
        if (curr == ')' || curr == '}') {
            if (brackets.empty()) {
                throw invalid_argument("invalid program, brackets do not match");
            }
            if ((curr == ')' && brackets.top() == '(') || (curr == '}' && brackets.top() == '{')) {
                brackets.pop();
            } else {
                throw invalid_argument("invalid program, brackets do not match");
            }
        }
    }
}

// special keywords

bool Lexer::isKeyword(string input) {
    return input == "read" || input == "print" || input == "call" || input == "while" ||
           input == "if" || input == "then" || input == "procedure" || input == "else";
}

bool Lexer::isAssignStmt(vector<string> content) {
    return find(content.begin(), content.end(), "=") != content.end();
}

bool Lexer::isStmt(vector<string> content) {
    return (
        (content.front() != "}" && content.front() != "else" && content.front() != "procedure") ||
        (isAssignStmt(content)));
}

// special symbols

bool Lexer::isOperator(char input) { // logical, comparison, artihmetic and
                                     // assignment (they all overlap)
    return input == '&' || input == '|' || input == '!' || input == '>' || input == '<' ||
           input == '=' || input == '+' || input == '-' || input == '*' || input == '/' ||
           input == '%';
}

bool Lexer::isBracket(char input) {
    return input == '(' || input == ')' || input == '{' || input == '}';
}

bool Lexer::isSemiColon(char input) { return input == ';'; }

bool Lexer::isCurlyBracket(string input) { return input == "{" || input == "}"; }

bool Lexer::isSemiColon(string input) { return input == ";"; }
