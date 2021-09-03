#pragma once
#include <string>
using namespace std;

class Parser {
public:
    vector<string> parseLine(string input);
    bool isStmt(vector<string> inputLine);
    bool isProc(vector<string> inputLine);
    bool isKeyword(string input);
    bool isBracket(char input);
    bool isOperator(char input);
    bool isSemiColon(char input);
};
