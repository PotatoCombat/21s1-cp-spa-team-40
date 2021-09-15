#include "source_processor/Parser.h"
#include <algorithm>
#include <iostream>
#include <tuple>

// parse file input

vector<Line> Parser::parseFile(fstream &file) {
    vector<Line> programLines = {};
    vector<vector<string>> programTokens = tokenizeFile(file);
    int stmtNum = 0;
    vector<string> currString = {};

    for (int i = 0; i < programTokens.size(); i++) {
        vector<string> subString = programTokens[i];
        currString.insert(currString.end(), subString.begin(), subString.end());

        if (!isCurlyBracket(currString.back()) &&
            !isSemiColon(currString.back())) {
            continue;
        }

        if (currString.front() != "}" && currString.front() != "else" &&
            currString.front() != "procedure") {
            stmtNum++;
        }

        Line curr = Line(stmtNum, currString);
        programLines.push_back(curr);
        currString = {};
    }

    return programLines;
}

vector<vector<string>> Parser::tokenizeFile(fstream &file) {
    string input;
    vector<vector<string>> programTokens = {};
    while (getline(file, input)) {
        vector<string> line = parseLine(input);
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

tuple<vector<string>, vector<string>> Parser::splitLine(vector<string> line) {
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

vector<string> Parser::parseLine(string input) {
    vector<string> inputLine = {};
    string currString = "";
    for (int i = 0; i < input.size(); i++) {
        char curr = input.at(i);
        currString = cleanString(currString);

        if (isOperator(curr) || isBracket(curr) || isSemiColon(curr)) {
            // push back previous string before special character
            addString(currString, inputLine);
            currString.push_back(curr);
            parseAndAddSymbol(input, i, curr, currString, inputLine);

        } else {
            currString.push_back(curr);
            if (isKeyword(currString)) {
                parseAndAddKeyword(input, i, currString, inputLine);
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

void Parser::parseAndAddSymbol(string input, int &i, char curr,
                               string &currString, vector<string> &inputLine) {
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

void Parser::parseAndAddKeyword(string input, int &i, string &currString,
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

void Parser::addString(string &input, vector<string> &inputVector) {
    input = cleanString(input);
    if (!input.empty()) {
        inputVector.push_back(input);
    }
    input = "";
}

string Parser::cleanString(string input) {
    input.erase(remove_if(input.begin(), input.end(), isspace), input.end());
    return input;
}

// special keywords

bool Parser::isProc(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "procedure") !=
           inputLine.end();
}

bool Parser::isKeyword(string input) {
    return input == "read" || input == "print" || input == "call" ||
           input == "while" || input == "if" || input == "then" ||
           input == "procedure";
}

// special symbols

bool Parser::isOperator(char input) { // logical, comparison, artihmetic and
                                      // assignment (they all overlap)
    return input == '&' || input == '|' || input == '!' || input == '>' ||
           input == '<' || input == '=' || input == '+' || input == '-' ||
           input == '*' || input == '/' || input == '%';
}

bool Parser::isBracket(char input) {
    return input == '(' || input == ')' || input == '{' || input == '}';
}

bool Parser::isSemiColon(char input) { return input == ';'; }

bool Parser::isArtihmeticOperator(string input) {
    return input == "+" || input == "-" || input == "*" || input == "/" ||
           input == "%";
}

bool Parser::isComparisonOperator(string input) {
    return input == ">" || input == ">=" || input == "<" || input == "<=" ||
           input == "==" || input == "!=";
}

bool Parser::isLogicalOperator(string input) {
    return input == "!" || input == "&&" || input == "||";
}

bool Parser::isCurlyBracket(string input) {
    return input == "{" || input == "}";
}

bool Parser::isRoundBracket(string input) {
    return input == "(" || input == ")";
}

bool Parser::isSemiColon(string input) { return input == ";"; }
