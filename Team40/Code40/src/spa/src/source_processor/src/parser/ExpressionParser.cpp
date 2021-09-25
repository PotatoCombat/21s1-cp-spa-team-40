#include "source_processor/parser/ExpressionParser.h"
#include <algorithm>

void ExpressionParser::parseExpression(vector<string> exprLst,
                                       Statement *stmt) {
    // if false, must be number. if true, must be operator
    bool oprtrFlag = false;
    vector<string> tempLst = exprLst;
    for (int i = 0; i < tempLst.size(); i++) { // remove brackets
        string currString = tempLst[i];
        if (isRoundBracket(currString)) {
            int start = i;
            string nextString = currString;
            while (currString == "(") {
                nextString = tempLst[i];
                i++;
                if (i == tempLst.size()) {
                    break;
                }
            }
            int end = i - 1;
            if (currString == "(") {

            } else if (currString == ")") {
            }
        }
    }
    for (int i = 0; i < tempLst.size(); i++) { // brackets removed
        string currString = tempLst[i];
        if (currString == "!") {
            oprtrFlag = false;
        } else if (!oprtrFlag) { // currString must be a factor
            handleFactor(stmt, currString);
            oprtrFlag = true;
        } else if (oprtrFlag) { // currString must be an operator
            handleOperator(stmt, currString);
            oprtrFlag = false;
        }
    }
}

void ExpressionParser::handleFactor(Statement *stmt, string currString) {
    if (isInteger(currString)) {
        auto constant = new ConstantValue(currString);
        stmt->addExpressionConst(constant);
    } else if (isName(currString)) {
        auto variable = new Variable(currString);
        stmt->addExpressionVar(variable);
    } else {
        throw invalid_argument("invalid variable or constant in statement");
    }
}

void ExpressionParser::handleOperator(Statement *stmt, string currString) {
    if (stmt->getStatementType() == StatementType::ASSIGN) {
        if (!isValidAssignOperator(stmt, currString)) {
            throw invalid_argument("invalid operator in assign statement");
        }
    } else if (stmt->getStatementType() == StatementType::WHILE) {
        if (!isValidWhileIfOperator(stmt, currString)) {
            throw invalid_argument("invalid operator in while statement");
        }
    } else if (stmt->getStatementType() == StatementType::IF) {
        if (!isValidWhileIfOperator(stmt, currString)) {
            throw invalid_argument("invalid operator in if statement");
        }
    } else {
        throw invalid_argument("invalid operator");
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
    // variables names are strings of letters, and digits, starting with
    // a letter
    if (!isalpha(input.at(0))) {
        return false;
    }
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isalnum(c)); }) == input.end();
}

bool ExpressionParser::isValidAssignOperator(Statement *stmt, string input) {
    return (isArtihmeticOperator(input));
}

bool ExpressionParser::isValidWhileIfOperator(Statement *stmt, string input) {
    return (isLogicalOperator(input) || isComparisonOperator(input) ||
            isArtihmeticOperator(input));
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
