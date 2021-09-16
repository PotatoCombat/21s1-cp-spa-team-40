#include "source_processor/parser/AssignStatementParser.h"
#include "source_processor/parser/ExpressionParser.h"
#include <algorithm>

AssignStatementParser::AssignStatementParser(vector<string> content, int index)
    : content(content), index(index) {
    stmt = new Statement(index, StatementType::ASSIGN);
};

Statement *AssignStatementParser::parseAssignStatement() {
    vector<string>::iterator assignItr =
        find(content.begin(), content.end(), "=");

    string var_name = *prev(assignItr);
    if (!isValidName(var_name)) {
        throw("invalid variable name");
    }
    auto variable = new Variable(var_name);
    stmt->setVariable(variable);

    vector<string>::iterator endItr = find(content.begin(), content.end(), ";");
    if (endItr == content.end())
        throw("invalid assign statement");

    vector<string> exprLst(next(assignItr), endItr);
    stmt->setExpressionLst(exprLst);
    ExpressionParser exprParser;
    exprParser.parseExpression(exprLst, stmt);
    return stmt;
}

bool AssignStatementParser::isValidName(string input) {
    // NAME: LETTER (LETTER | DIGIT)*
    // procedure names and variables are strings of letters, and digits,
    // starting with a letter
    if (!isalpha(input.at(0))) {
        return false;
    }
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isalnum(c)); }) == input.end();
}
