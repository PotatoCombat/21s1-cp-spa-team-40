#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "Parser.h"
#include "TNode.h"

vector<string> Parser::parseLine(string input) {
    vector<string> inputLine;
    for (int i = 0; i < input.size(); i++) {
        char curr = input.at(i);
    }
    return inputLine;
}

bool Parser::isStmt(vector<string> inputLine) {}

bool Parser::isProc(vector<string> inputLine) {}

bool Parser::isOperator(char input) {
    return input == '&' || input == '|' || input == '!' || input == '>' ||
           input == '<' || input == '=' || input == '+' || input == '-' ||
           input == '*' || input == '/' || input == '%';
}

bool Parser::isBracket(char input) {
    return input == '(' || input == ')' || input == '{' || input == '}';
}

bool Parser::isSemiColon(char input) { return input == ';'; }
