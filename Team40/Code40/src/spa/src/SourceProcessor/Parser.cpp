#include "Parser.h"
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

vector<string> Parser::parseLine(string input) {
    vector<string> inputLine;
    string currString = "";
    for (int i = 0; i < input.size(); i++) {
        char curr = input.at(i);

        if (isOperator(curr) || isBracket(curr) || isSemiColon(curr)) {
            if (currString.length() != 0) {
                inputLine.push_back(clean(currString));
                currString = "";
            }
            inputLine.push_back(clean(string(1, curr)));

        } else {
            currString.push_back(curr);
            if (isKeyword(currString)) {
                inputLine.push_back(clean(currString));
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

bool Parser::isStmt(vector<string> inputLine) { return true; }

bool Parser::isProc(vector<string> inputLine) { return true; }

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
