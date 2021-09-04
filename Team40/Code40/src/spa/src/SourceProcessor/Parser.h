#pragma once
#include "../common/model/Procedure.h"
#include "../common/model/Program.h"
#include "Line.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Parser {
public:
    // parse file input
    vector<Line> parseFile(fstream &file);
    vector<string> parseLine(string input);
    Program parseProgram(vector<Line> programLines);
    string clean(string input);

    // parse preprocessed file
    Procedure parseProcedure(vector<string> content);

    // special keywords
    bool isProc(vector<string> inputLine);
    bool isKeyword(string input);

    // special characters
    bool isBracket(char input);
    bool isOperator(char input);
    bool isSemiColon(char input);
};
