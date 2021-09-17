#pragma once

#include "common/model/ConstantValue.h"
#include "common/model/Statement.h"
#include "common/model/Variable.h"
#include <string>
#include <vector>

using namespace std;

class ExpressionParser {
public:
    void parseExpression(vector<string> exprLst, Statement *stmt);
    void handleOperator(Statement *stmt, string currString);
    void handleFactor(Statement *stmt, string currString);
    bool isInteger(string input);
    bool isName(string input);
    bool isValidAssignOperator(Statement *stmt, string input);
    bool isValidWhileIfOperator(Statement *stmt, string input);
    bool isRoundBracket(string input);
    bool isArtihmeticOperator(string input);
    bool isComparisonOperator(string input);
    bool isLogicalOperator(string input);
};