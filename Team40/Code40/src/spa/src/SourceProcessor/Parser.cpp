#include "Parser.h"
#include "Line.cpp"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

vector<Line> Parser::parseFile(fstream &file) {
    vector<Line> programLines = {};
    string input;
    int stmtNum;
    while (getline(file, input)) {
        vector<string> inputLine = parseLine(input);
        if (isProc(inputLine)) {
            stmtNum = 0;
        } else {
            stmtNum++;
        }
        Line curr = Line(stmtNum, inputLine);
        programLines.push_back(curr);
    }
    return programLines;
}

vector<string> Parser::parseLine(string input) {
    vector<string> inputLine;
    string currString = "";
    for (int i = 0; i < input.size(); i++) {
        char curr = input.at(i);
        currString = clean(currString);

        if (isOperator(curr) || isBracket(curr) || isSemiColon(curr)) {
            if (currString.length() != 0) {
                if (!currString.empty()) {
                    inputLine.push_back(currString);
                }
                currString = "";
            }
            if (!string(1, curr).empty()) {
                inputLine.push_back(string(1, curr));
            }

        } else {
            currString.push_back(curr);
            if (isKeyword(currString)) {
                if (!currString.empty()) {
                    inputLine.push_back(currString);
                }
                currString = "";
            }
        }
    }
    return inputLine;
}

string Parser::clean(string input) {
    input.erase(remove(input.begin(), input.end(), ' '), input.end());
    return input;
}

bool Parser::isProc(vector<string> inputLine) {
    return find(inputLine.begin(), inputLine.end(), "procedure") !=
           inputLine.end();
}

bool Parser::isKeyword(string input) {
    return input == "read" || input == "print" || input == "call" ||
           input == "while" || input == "if" || input == "then" ||
           input == "procedure";
}

// special characters

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
