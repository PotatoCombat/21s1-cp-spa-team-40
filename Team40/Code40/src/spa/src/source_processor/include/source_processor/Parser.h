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
    vector<vector<string>> tokenizeFile(fstream &file);
    tuple<vector<string>, vector<string>> splitLine(vector<string> inputLine);
    vector<string> parseLine(string input);

    // helper functions
    void parseAndAddSymbol(string input, int &index, char curr,
                           string &currString, vector<string> &inputLine);
    void parseAndAddKeyword(string input, int &index, string &currString,
                            vector<string> &inputLine);
    string cleanString(string input);
    void addString(string &input, vector<string> &inputVector);

    // special keywords
    bool isProc(vector<string> inputLine);
    bool isKeyword(string input);

    // special symbols
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
