#pragma once
#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Variable.h"
#include "common/model/condition/Condition.h"
#include "common/model/expression/Expression.h"
#include "source_processor/Line.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Parser {
public:
    // parse file input
    vector<Line> parseFile(fstream &file);
    vector<string> parseLine(string input);
    string cleanString(string input);
    void addString(string &input, vector<string> &inputVector);

    // parse preprocessed file
    Program parseProgram(vector<Line> programLines);
    Statement parseStatement(vector<string> content, int index,
                             vector<Line> programLines, int programIndex);

    // special keywords
    bool isProc(vector<string> inputLine);
    bool isKeyword(string input);

    // special characters
    bool isBracket(char input);
    bool isOperator(char input);
    bool isSemiColon(char input);
    bool isCurlyBracket(string input);
    bool isRoundBracket(string input);
    bool isArtihmeticOperator(string input);
    bool isComparisonOperator(string input);
    bool isLogicalOperator(string input);
    bool isSemiColon(string input);
    bool isInteger(string input);
    bool isName(string input);
};
