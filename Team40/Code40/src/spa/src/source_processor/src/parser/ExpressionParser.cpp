#include "source_processor/parser/ExpressionParser.h"
#include <algorithm>

ExpressionParser::ExpressionParser(vector<string> exprLst, Statement *stmt)
    : exprLst(exprLst), stmt(stmt){};

vector<string> ExpressionParser::parseExpression() {
    checkValidExpression();
    bool oprtrFlag = false; // if false, must be variable/constant. if true, must be operator
    for (int i = 0; i < exprLst.size(); i++) {
        string curr = exprLst[i];
        if (isInteger(curr) && !oprtrFlag) {
            ConstantValue *constant = new ConstantValue(curr);
            stmt->addExpressionConst(constant);
            oprtrFlag = true; // next token must be an operator
        } else if (isName(curr) && !oprtrFlag) {
            Variable *variable = new Variable(curr);
            stmt->addExpressionVar(variable);
            oprtrFlag = true; // next token must be an operator
        } else if (isRoundBracket(curr)) {
            checkValidBracket(curr, i);
        } else if ((isOperator(curr) && oprtrFlag) || (curr == "!")) {
            checkValidOperator(curr, i);
            oprtrFlag = false; // next token must be a variable/constant
        } else {
            throw invalid_argument(
                "invalid expression: invalid variable, constant or operator encountered");
        }
    }
    if (!brackets.empty()) {
        throw invalid_argument("invalid expression: brackets do not match");
    }
    return exprLst;
}

void ExpressionParser::checkValidOperator(string curr, int index) {
    if (stmt->getStatementType() == StatementType::ASSIGN) {
        if (!isValidAssignOperator(curr)) {
            throw invalid_argument("invalid expression: invalid operator in assign statement");
        }
    } else if (stmt->getStatementType() == StatementType::WHILE) {
        if (!isValidConditionalOperator(curr)) {
            throw invalid_argument("invalid expression: invalid operator in while statement");
        }
    } else if (stmt->getStatementType() == StatementType::IF) {
        if (!isValidConditionalOperator(curr)) {
            throw invalid_argument("invalid expression: invalid operator in if statement");
        }
    }
    if (isLogicalOperator(curr)) {
        if (index == exprLst.size() - 1) {
            throw invalid_argument("invalid expression: ( must appear after logical operator");
        } else if (exprLst[index + 1] != "(") {
            throw invalid_argument("invalid expression: ( must appear after logical operator");
        }
        if (curr != "!") {
            if (index == 0) {
                throw invalid_argument("invalid expression: ) must appear before logical operator");
            } else if (exprLst[index - 1] != ")") {
                throw invalid_argument("invalid expression: ) must appear before logical operator");
            }
        }
    }
}

void ExpressionParser::checkValidBracket(string curr, int index) {
    if (curr == "(") {
        brackets.push(curr);
    } else if (curr == ")") {
        if (brackets.empty()) {
            throw invalid_argument("invalid expression: brackets do not match");
        } else if (brackets.top() == "(") {
            brackets.pop();
        } else {
            throw invalid_argument("invalid expression: brackets do not match");
        }
    }
    int start = index;
    int end = index;
    while (exprLst[start] == curr) {
        start--;
        if (start < 0)
            break;
    }
    while (exprLst[end] == curr) {
        end++;
        if (end >= exprLst.size())
            break;
    }
    start++;
    end--;
    if (curr == "(")
        checkValidOpenBracket(start, end);
    else if (curr == ")")
        checkValidCloseBracket(start, end);
}

void ExpressionParser::checkValidOpenBracket(int start, int end) {
    if (start == 0 && end == exprLst.size() - 1) {
        return;
    } else if (start == 0) {
        string next = exprLst[end + 1];
        if (!(isInteger(next) || isName(next) || next == "!")) {
            throw invalid_argument("invalid expression: factor or ! must appear after (");
        }
    } else if (end == exprLst.size() - 1) {
        throw invalid_argument("invalid expression: factor or ! must appear after (");
    } else {
        string prev = exprLst[start - 1];
        string next = exprLst[end + 1];
        if (!(isOperator(prev) && (isInteger(next) || isName(next) || next == "!"))) {
            throw invalid_argument("invalid expression: factor or ! must appear after ( and "
                                   "operator must appear before (");
        }
    }
}

void ExpressionParser::checkValidCloseBracket(int start, int end) {
    if (start == 0 && end == exprLst.size() - 1) {
        return;
    } else if (start == 0) {
        throw invalid_argument("invalid expression: factor or ! must appear before )");
    } else if (end == exprLst.size() - 1) {
        string prev = exprLst[start - 1];
        if (!(isInteger(prev) || isName(prev))) {
            throw invalid_argument("invalid expression: factor or ! must appear before )");
        }
    } else {
        string prev = exprLst[start - 1];
        string next = exprLst[end + 1];
        if (!(isOperator(next) && (isInteger(prev) || isName(prev)))) {
            throw invalid_argument("invalid expression: factor or ! must appear before ) and "
                                   "operator must appear after )");
        }
    }
}

void ExpressionParser::checkValidExpression() {
    if (exprLst.empty()) {
        throw invalid_argument("invalid expression: no arguments");
    }
    // conditions and expressions should not end with an operator
    if (isOperator(exprLst.back())) {
        throw invalid_argument("invalid expression: cannot end with an operator");
    }
    StatementType type = stmt->getStatementType();
    // condition should have at least one operator
    if (type == StatementType::IF || type == StatementType::WHILE) {
        if ((exprLst.size() < 3) || ((exprLst.size() < 2) && (exprLst[0] != "!"))) {
            throw invalid_argument("invalid expression: conditions need at least one operator");
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

bool ExpressionParser::isValidAssignOperator(string input) { return (isArtihmeticOperator(input)); }

bool ExpressionParser::isValidConditionalOperator(string input) {
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
