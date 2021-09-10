#include "source_processor/Parser.h"
#include "source_processor/AssignStatementParser.h"
#include "source_processor/CallStatementParser.h"
#include "source_processor/IfStatementParser.h"
#include "source_processor/PrintStatementParser.h"
#include "source_processor/ProcedureParser.h"
#include "source_processor/ReadStatementParser.h"
#include "source_processor/StatementParser.h"
#include "source_processor/WhileStatementParser.h"
#include <algorithm>
#include <iostream>

// parse file input

vector<Line> Parser::parseFile(fstream &file) {
    vector<Line> programLines = {};
    string input;
    int stmtNum = 0;
    vector<string> currString = {};
    vector<string> nextString = {};
    while (getline(file, input)) {
        currString = nextString;
        nextString = {};
        for (int i = 0; i < currString.size(); i++) {
            string curr = currString[i];
            if ((isCurlyBracket(curr)) || isSemiColon(curr)) {
                currString.push_back(curr);
                nextString = vector<string>(currString.begin() + i + 1,
                                            currString.end());
                break;
            }
            currString.push_back(curr);
        }
        if (!currString.empty() && currString[0] != "}" &&
            currString[0] != "else" && !isProc(currString)) {
            stmtNum++;
        }
        if (!currString.empty()) {
            Line curr = Line(stmtNum, currString);
            programLines.push_back(curr);
        }
        currString = {};
        vector<string> inputLine = parseLine(input);
        for (int i = 0; i < inputLine.size(); i++) {
            string curr = inputLine[i];
            if ((isCurlyBracket(curr)) || isSemiColon(curr)) {
                currString.push_back(curr);
                nextString =
                    vector<string>(inputLine.begin() + i + 1, inputLine.end());
                break;
            }
            currString.push_back(curr);
        }
        if (!currString.empty() && currString[0] != "}" &&
            currString[0] != "else" && !isProc(currString)) {
            stmtNum++;
        }
        if (!currString.empty()) {
            Line curr = Line(stmtNum, currString);
            programLines.push_back(curr);
        }
    }
    return programLines;
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
            parseSymbol(input, i, curr, currString, inputLine);
        } else {
            currString.push_back(curr);
            if (isKeyword(currString)) {
                parseKeyword(input, i, currString, inputLine);
            }
        }
    }
    return inputLine;
}

void Parser::parseSymbol(string input, int &i, char curr, string &currString,
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

void Parser::parseKeyword(string input, int &i, string &currString,
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
    if (!input.empty()) {
        inputVector.push_back(input);
    }
    input = "";
}

string Parser::cleanString(string input) {
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
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

bool Parser::isInteger(string input) {
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isdigit(c)); }) == input.end();
}

bool Parser::isName(string input) {
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isalnum(c)); }) == input.end();
}
