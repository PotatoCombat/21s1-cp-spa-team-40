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
    // for bracket matching
    stack<string> brackets;
    stack<string> operators;

    // cond_expr: rel_expr | '!' '(' cond_expr ')' | '(' cond_expr ')' '&&' '(' cond_expr ')' |
    // '('cond_expr ')' '||' '(' cond_expr ')'
    // rel_expr: rel_factor '>' rel_factor | rel_factor '>=' rel_factor | rel_factor '<' rel_factor
    // | rel_factor '<=' rel_factor | rel_factor '==' rel_factor | rel_factor '!=' rel_factor
    // expr: expr '+' term | expr '-' term | term
    // term: term '*' factor | term '/' factor | term '%' factor | factor
    unordered_map<string, int> precedenceMap = {
        {"!", 1}, {"||", 1}, {"&&", 1},                                  // logical operators
        {">", 2}, {">=", 2}, {"<", 2},  {"<=", 2}, {"==", 2}, {"!=", 2}, // comparison operators
        {"+", 3}, {"-", 3},  {"%", 4},  {"*", 4},  {"/", 4}};            // arithmetic operators

    void parseExpression(vector<string> exprLst, Statement *stmt);
    void handleBracket(string curr);
    void handleOperator(string curr, Statement *stmt);
    void checkValidBracket(string curr);

    bool isInteger(string input);
    bool isName(string input);
    bool isValidAssignOperator(Statement *stmt, string input);
    bool isValidWhileIfOperator(Statement *stmt, string input);
    bool isOperator(string input);
    bool isArtihmeticOperator(string input);
    bool isComparisonOperator(string input);
    bool isLogicalOperator(string input);
};