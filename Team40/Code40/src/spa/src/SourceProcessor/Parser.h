#pragma once
#include "Line.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Parser {
public:
    vector<Line> parseFile(fstream &file);
    vector<string> parseLine(string input);
    bool isProc(vector<string> inputLine);
    bool isKeyword(string input);
    string clean(string input);
    bool isBracket(char input);
    bool isOperator(char input);
    bool isSemiColon(char input);
};
