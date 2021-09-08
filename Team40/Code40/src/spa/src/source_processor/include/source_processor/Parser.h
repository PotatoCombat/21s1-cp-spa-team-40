#pragma once
#include "Line.h"
#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Variable.h"
#include "common/model/condition/Condition.h"
#include "common/model/expression/Expression.h"
#include "common/model/statement/IfStatement.h"
#include "common/model/statement/Statement.h"
#include "common/model/statement/WhileStatement.h"
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
    Procedure parseProcedure(vector<string> content);
    Statement parseStatement(vector<string> content, int index,
                             vector<Line> programLines, int programIndex);
    Statement parseReadStatement(vector<string> content, int index);
    Statement parsePrintStatement(vector<string> content, int index);
    Statement parseCallStatement(vector<string> content, int index);
    Statement parseAssignStatement(vector<string> content, int index);
    Statement parseWhileStatement(vector<string> content, int index,
                                  vector<Line> programLines, int programIndex);
    Statement parseIfStatement(vector<string> content, int index,
                               vector<Line> programLines, int programIndex);

    // special keywords
    bool isProc(vector<string> inputLine);
    bool isReadStmt(vector<string> inputLine);
    bool isPrintStmt(vector<string> inputLine);
    bool isCallStmt(vector<string> inputLine);
    bool isWhileStmt(vector<string> inputLine);
    bool isAssignStmt(vector<string> inputLine);
    bool isIfStmt(vector<string> inputLine);
    bool isKeyword(string input);
    bool hasTerminator(vector<string> inputLine);

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
