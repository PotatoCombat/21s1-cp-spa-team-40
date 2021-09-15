#include "source_processor/AssignStatementParser.h"
#include "source_processor/ExpressionParser.h"
#include <algorithm>

AssignStatementParser::AssignStatementParser(vector<string> content, int index)
    : content(content), index(index) {
    stmt = new Statement(index, StatementType::ASSIGN);
};

Statement *AssignStatementParser::parseAssignStatement() {
    vector<string>::iterator assignItr =
        find(content.begin(), content.end(), "=");
    string var_name = *prev(assignItr);
    auto variable = new Variable(var_name);
    stmt->setVariable(variable);

    vector<string>::iterator endItr = find(content.begin(), content.end(), ";");
    vector<string> exprLst(next(assignItr), endItr);
    stmt->setExpressionLst(exprLst);
     ExpressionParser exprParser;
     exprParser.parseExpression(exprLst, stmt);
    return stmt;
}
