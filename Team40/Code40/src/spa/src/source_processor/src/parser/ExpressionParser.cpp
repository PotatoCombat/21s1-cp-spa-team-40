#include "source_processor/parser/ExpressionParser.h"
#include "source_processor/parser/Tokens.h"
#include <algorithm>

ExpressionParser::ExpressionParser(vector<string> exprLst, Statement *stmt)
    : exprLst(exprLst), stmt(stmt){};

/**
 * Parses the conditional expressions in If and While statements as well as the
 * arithmetic expressions in Assign statements into an expression list. Also
 * ensures that the expression is valid by ensuring that operators and factors
 * are alternating
 * @return Parsed expression list.
 */
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
        } else if (Tokens::isRoundBracket(curr)) {
            checkValidBracket(curr, i);
        } else if ((Tokens::isOperator(curr) && oprtrFlag) || (curr == Tokens::SYMBOL_NOT)) {
            checkValidOperator(curr, i);
            oprtrFlag = false; // next token must be a variable/constant
        } else {
            throw invalid_argument("invalid expression: invalid variable, "
                                   "constant or operator encountered");
        }
    }
    if (!brackets.empty()) {
        throw invalid_argument("invalid expression: brackets do not match");
    }
    return exprLst;
}

/**
 * Ensures that a detected operator is valid according to the statement type
 */
void ExpressionParser::checkValidOperator(string curr, int index) {
    if (stmt->getStatementType() == StatementType::ASSIGN) {
        if (!Tokens::isValidAssignOperator(curr)) {
            throw invalid_argument("invalid expression: invalid operator in assign statement");
        }
    } else if (stmt->getStatementType() == StatementType::WHILE) {
        if (!Tokens::isValidConditionalOperator(curr)) {
            throw invalid_argument("invalid expression: invalid operator in while statement");
        }
    } else if (stmt->getStatementType() == StatementType::IF) {
        if (!Tokens::isValidConditionalOperator(curr)) {
            throw invalid_argument("invalid expression: invalid operator in if statement");
        }
    }
    if (Tokens::isLogicalOperator(curr)) {
        if (index == exprLst.size() - 1) {
            throw invalid_argument("invalid expression: ( must appear after logical operator");
        } else if (exprLst[index + 1] != Tokens::SYMBOL_OPEN_BRACKET) {
            throw invalid_argument("invalid expression: ( must appear after logical operator");
        }
        if (curr != Tokens::SYMBOL_NOT) {
            if (index == 0) {
                throw invalid_argument("invalid expression: ) must appear "
                                       "before logical operator");
            } else if (exprLst[index - 1] != Tokens::SYMBOL_CLOSE_BRACKET) {
                throw invalid_argument("invalid expression: ) must appear "
                                       "before logical operator");
            }
        }
    }
}

/**
 * Ensures that the entire expression has valid brackets that match each other
 */
void ExpressionParser::checkValidBracket(string curr, int index) {
    if (curr == Tokens::SYMBOL_OPEN_BRACKET) {
        brackets.push(curr);
    } else if (curr == Tokens::SYMBOL_CLOSE_BRACKET) {
        if (brackets.empty()) {
            throw invalid_argument("invalid expression: brackets do not match");
        } else if (brackets.top() == Tokens::SYMBOL_OPEN_BRACKET) {
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
    if (curr == Tokens::SYMBOL_OPEN_BRACKET)
        checkValidOpenBracket(start, end);
    else if (curr == Tokens::SYMBOL_CLOSE_BRACKET)
        checkValidCloseBracket(start, end);
}

/**
 * Ensures that a detected open bracket is followed by either a factor or a not
 * operator and preceded by nothing or an operator. Also ensures that an
 * expression (min 3 tokens) exists between the current open bracket and the
 * next close bracket.
 */
void ExpressionParser::checkValidOpenBracket(int start, int end) {
    if (start == 0 && end == exprLst.size() - 1) {
        return;
    } else if (start == 0) {
        string next = exprLst[end + 1];
        if (!(isInteger(next) || isName(next) || next == Tokens::SYMBOL_NOT)) {
            throw invalid_argument("invalid expression: factor or ! must appear after (");
        }
    } else if (end == exprLst.size() - 1) {
        throw invalid_argument("invalid expression: factor or ! must appear after (");
    } else {
        string prev = exprLst[start - 1];
        string next = exprLst[end + 1];
        if (!(Tokens::isOperator(prev) &&
              (isInteger(next) || isName(next) || next == Tokens::SYMBOL_NOT))) {
            throw invalid_argument("invalid expression: factor or ! must appear after ( and "
                                   "operator must appear before (");
        }
    }
}

/**
 * Ensures that a detected close bracket is preceded by a factor and followed by
 * nothing or an operator. Also ensures that an expression (min 3 tokens) exists
 * between the current close bracket and the previous open bracket.
 */
void ExpressionParser::checkValidCloseBracket(int start, int end) {
    if (start == 0 && end == exprLst.size() - 1) {
        return;
    } else if (start == 0) {
        throw invalid_argument("invalid expression: factor must appear before )");
    } else if (end == exprLst.size() - 1) {
        string prev = exprLst[start - 1];
        if (!(isInteger(prev) || isName(prev))) {
            throw invalid_argument("invalid expression: factor must appear before )");
        }
    } else {
        string prev = exprLst[start - 1];
        string next = exprLst[end + 1];
        if (!(Tokens::isOperator(next) && (isInteger(prev) || isName(prev)))) {
            throw invalid_argument("invalid expression: factor must appear before ) and "
                                   "operator must appear after )");
        }
    }
}

/**
 * Ensures that the expression is not empty and does not terminate with an
 * operator. Also ensures that conditional expressions contain at least one
 * logical or comparison operator
 */
void ExpressionParser::checkValidExpression() {
    if (exprLst.empty()) {
        throw invalid_argument("invalid expression: no arguments");
    }
    if (stmt->getStatementType() == StatementType::IF) {
        if (find_if(exprLst.begin(), exprLst.end(), [](string s) {
                return (Tokens::isLogicalOperator(s) || Tokens::isComparisonOperator(s));
            }) == exprLst.end()) {
            throw invalid_argument("invalid expression: conditional expressions should have at "
                                   "least one logical or comparison operator");
        }
    }
    // conditions and expressions should not end with an operator
    if (Tokens::isOperator(exprLst.back())) {
        throw invalid_argument("invalid expression: cannot end with an operator");
    }
}

bool ExpressionParser::isInteger(string input) {
    // INTEGER: DIGIT+
    // constants are sequences of digits
    if ((input.length() > 2) && (input[0] == '0')) {
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
