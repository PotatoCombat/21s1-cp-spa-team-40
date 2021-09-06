#pragma once
#include "../common/model/ConstantValue.h"
#include "../common/model/Procedure.h"
#include "../common/model/Program.h"
#include "../common/model/Variable.h"
#include "../common/model/condition/Condition.h"
#include "../common/model/expression/Expression.h"
#include "../common/model/statement/Statement.h"
#include "Line.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Parser {
private:
    Program program;
    vector<Variable> varLst;
    vector<ConstantValue> constLst;

public:
    // parse file input
    vector<Line> parseFile(fstream &file);
    vector<string> parseLine(string input);
    string clean(string input);

    // parse preprocessed file
    Program parseProgram(vector<Line> programLines);
    Procedure parseProcedure(vector<string> content);
    Statement parseStatement(vector<string> content, int index);
    Statement parseReadStatement(vector<string> content, int index);
    Statement parsePrintStatement(vector<string> content, int index);
    Statement parseCallStatement(vector<string> content, int index);
    Statement parseWhileStatement(vector<string> content, int index);
    Statement parseAssignStatement(vector<string> content, int index);
    Expression parseExpression(vector<string>::iterator exprItr);
    Condition parseCondition(vector<string>::iterator condItr);

    // special keywords
    bool isProc(vector<string> inputLine);
    bool isReadStmt(vector<string> inputLine);
    bool isPrintStmt(vector<string> inputLine);
    bool isCallStmt(vector<string> inputLine);
    bool isWhileStmt(vector<string> inputLine);
    bool isAssignStmt(vector<string> inputLine);
    bool isIfStmt(vector<string> inputLine);
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

    // modifiers
    void addToVarLst(Variable var);
    void addToConstLst(ConstantValue constVal);

    // getters
    vector<Variable> getVarLst();
    vector<ConstantValue> getConstLst();
    Program getProgram();
};
