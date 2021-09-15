#include "source_processor/ExpressionParser.h"
#include <algorithm>
#include <stdexcept>

void ExpressionParser::parseExpression(vector<string> exprLst,
                                       Statement *stmt) {
    for (int i = 0; i < exprLst.size(); i++) {
        string currString = exprLst[i];
        if (isInteger(currString)) {
            auto constant = new ConstantValue(stoi(currString));
            stmt->addExpressionConst(constant);
        } else if (isName(currString)) {
            auto variable = new Variable(currString);
            stmt->addExpressionVar(variable);
        } else if (stmt->getStatementType() == StatementType::ASSIGN) {
            if (!isValidAssignOperator(stmt, currString)) {
                throw runtime_error(
                    "Invalid SIMPLE program: invalid variable or "
                    "constant in assign statement");
            }
        } else if (stmt->getStatementType() == StatementType::WHILE ||
                   stmt->getStatementType() == StatementType::IF) {
            if (!isValidWhileIfOperator(stmt, currString)) {
                throw runtime_error(
                    "Invalid SIMPLE program: invalid variable or "
                    "constant in while or if statement");
            }
        }
    }
}

bool ExpressionParser::isInteger(string input) {
    // INTEGER: DIGIT+
    // constants are sequences of digits
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isdigit(c)); }) == input.end();
}

bool ExpressionParser::isName(string input) {
    // NAME: LETTER (LETTER | DIGIT)*
    // procedure names and variables are strings of letters, and digits,
    // starting with a letter
    if (!isalpha(input.at(0))) {
        return false;
    }
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isalnum(c)); }) == input.end();
}

bool ExpressionParser::isValidAssignOperator(Statement *stmt, string input) {
    return (isArtihmeticOperator(input) || isRoundBracket(input));
}

bool ExpressionParser::isValidWhileIfOperator(Statement *stmt, string input) {
    return (isLogicalOperator(input) || isComparisonOperator(input) ||
             isRoundBracket(input));
}

bool ExpressionParser::isRoundBracket(string input) {
    return input == "(" || input == ")";
}

bool ExpressionParser::isArtihmeticOperator(string input) {
    return input == "+" || input == "-" || input == "*" || input == "/" ||
           input == "%";
}

bool ExpressionParser::isComparisonOperator(string input) {
    return input == ">" || input == ">=" || input == "<" || input == "<=" ||
           input == "==" || input == "!=";
}

bool ExpressionParser::isLogicalOperator(string input) {
    return input == "!" || input == "&&" || input == "||";
}
