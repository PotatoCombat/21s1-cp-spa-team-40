#pragma once

#include "common/model/ConstantValue.h"
#include "common/model/Statement.h"
#include "common/model/Variable.h"
#include <stack>
#include <string>
#include <unordered_map>

using namespace std;

class ExpressionParser {
public:
    ExpressionParser(vector<string> exprLst, Statement *stmt);

    vector<string> exprLst; 
    Statement *stmt;
    stack<string> brackets; // for bracket matching

    vector<string> parseExpression();
    void checkValidOperator(string curr, int index);
    void checkValidBracket(string curr, int index);
    void checkValidOpenBracket(int start, int end);
    void checkValidCloseBracket(int start, int end);
    void checkValidExpression();

    bool isInteger(string input);
    bool isName(string input);
    bool isRoundBracket(string input);
    bool isValidAssignOperator(string input);
    bool isValidConditionalOperator(string input);
    bool isOperator(string input);
    bool isArtihmeticOperator(string input);
    bool isComparisonOperator(string input);
    bool isLogicalOperator(string input);
};