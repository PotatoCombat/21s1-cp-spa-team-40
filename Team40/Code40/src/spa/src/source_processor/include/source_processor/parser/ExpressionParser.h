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

    static const int MIN_COND_LEN_WITH_NOT = 2;
    static const int MIN_COND_LEN_WITH_AND_OR = 3;

    vector<string> parseExpression();
    void checkValidOperator(string curr, int index);
    void checkValidBracket(string curr, int index);
    void checkValidOpenBracket(int start, int end);
    void checkValidCloseBracket(int start, int end);
    void checkValidExpression();

    bool isInteger(string input);
    bool isName(string input);
};
