#pragma once
#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Variable.h"
#include "common/model/condition/Condition.h"
#include "common/model/expression/Expression.h"
#include "source_processor/Line.h"
#include <fstream>
#include <map>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class Lexer {
public:
    // for bracket matching
    stack<char> brackets;

    // tokenize file input
    vector<Line> tokenizeFile(fstream &file);
    vector<vector<string>> mergeLine(fstream &file);
    tuple<vector<string>, vector<string>> splitLine(vector<string> inputLine);
    vector<string> tokenizeLine(string input);

    // helper functions
    void tokenizeAndAddSymbol(string input, int &index, char curr,
                              string &currString, vector<string> &inputLine);
    void tokenizeAndAddKeyword(string input, int &index, string &currString,
                               vector<string> &inputLine);
    string cleanString(string input);
    void addString(string &input, vector<string> &inputVector);
    void checkValidBracket(char curr);

    // special keywords
    bool isKeyword(string input);

    // special symbols
    bool isBracket(char input);
    bool isOperator(char input);
    bool isSemiColon(char input);
    bool isCurlyBracket(string input);
    bool isSemiColon(string input);
};
