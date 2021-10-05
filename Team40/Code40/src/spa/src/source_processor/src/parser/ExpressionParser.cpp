#include "source_processor/parser/ExpressionParser.h"
#include <algorithm>

void ExpressionParser::parseExpression(vector<string> exprLst, Statement *stmt) {
    bool oprtrFlag = false; // if false, must be variable/constant. if true, must be operator
    for (int i = 0; i < exprLst.size(); i++) {
        string curr = exprLst[i];
        if (isInteger(curr) && !oprtrFlag) {
            auto constant = new ConstantValue(curr);
            stmt->addExpressionConst(constant);
            oprtrFlag = true; // next token must be an operator
        } else if (isName(curr) && !oprtrFlag) {
            auto variable = new Variable(curr);
            stmt->addExpressionVar(variable);
            oprtrFlag = true; // next token must be an operator
        } else if (isRoundBracket(curr)) {
            checkValidBracket(curr);
        } else if ((isOperator(curr) && oprtrFlag) || (curr == "!")) {
            checkValidOperator(curr, stmt);
            oprtrFlag = false; // next token must be a variable/constant
        } else {
            throw invalid_argument(
                "invalid expression: invalid variable, constant or operator encountered");
        }
    }
    if (!brackets.empty()) {
        throw invalid_argument("invalid expression, brackets do not match");
    }
}

void ExpressionParser::checkValidOperator(string curr, Statement *stmt) {
    if (stmt->getStatementType() == StatementType::ASSIGN) {
        if (!isValidAssignOperator(stmt, curr)) {
            throw invalid_argument("invalid operator in assign statement");
        }
    } else if (stmt->getStatementType() == StatementType::WHILE) {
        if (!isValidWhileIfOperator(stmt, curr)) {
            throw invalid_argument("invalid operator in while statement");
        }
    } else if (stmt->getStatementType() == StatementType::IF) {
        if (!isValidWhileIfOperator(stmt, curr)) {
            throw invalid_argument("invalid operator in if statement");
        }
    } else {
        throw invalid_argument("invalid operator");
    }
}

void ExpressionParser::checkValidBracket(string curr) {
    if (curr == "(") {
        brackets.push(curr);
    } else if (curr == ")") {
        if (brackets.empty()) {
            throw invalid_argument("invalid expression, brackets do not match");
        } else if (brackets.top() == "(") {
            brackets.pop();
        } else {
            throw invalid_argument("invalid expression, brackets do not match");
        }
    }
}

bool ExpressionParser::isInteger(string input) {
    // INTEGER: DIGIT+
    // constants are sequences of digits
    if ((input.length() > 0) && (input[0] == 0)) {
        return false;
    }
    return find_if(input.begin(), input.end(), [](char c) { return !(isdigit(c)); }) == input.end();
}

bool ExpressionParser::isName(string input) {
    // NAME: LETTER (LETTER | DIGIT)*
    // variables names are strings of letters, and digits, starting with
    // a letter
    if (!isalpha(input.at(0))) {
        return false;
    }
    return find_if(input.begin(), input.end(), [](char c) { return !(isalnum(c)); }) == input.end();
}

bool ExpressionParser::isRoundBracket(string input) { return input == "(" || input == ")"; }

bool ExpressionParser::isValidAssignOperator(Statement *stmt, string input) {
    return (isArtihmeticOperator(input));
}

bool ExpressionParser::isValidWhileIfOperator(Statement *stmt, string input) {
    return (isLogicalOperator(input) || isComparisonOperator(input) || isArtihmeticOperator(input));
}

bool ExpressionParser::isOperator(string input) {
    return isLogicalOperator(input) || isComparisonOperator(input) || isArtihmeticOperator(input);
}

bool ExpressionParser::isArtihmeticOperator(string input) {
    return input == "+" || input == "-" || input == "*" || input == "/" || input == "%";
}

bool ExpressionParser::isComparisonOperator(string input) {
    return input == ">" || input == ">=" || input == "<" || input == "<=" || input == "==" ||
           input == "!=";
}

bool ExpressionParser::isLogicalOperator(string input) {
    return input == "!" || input == "&&" || input == "||";
}
