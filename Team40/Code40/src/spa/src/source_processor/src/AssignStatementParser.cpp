#include "source_processor/AssignStatementParser.h"
#include "common/model/statement/AssignStatement.h"
#include <algorithm>

AssignStatementParser::AssignStatementParser(vector<string> content, int index)
    : StatementParser(content, index, {}, -1){};

Statement AssignStatementParser::parseAssignStatement() {
    vector<string>::iterator assignItr =
        find(content.begin(), content.end(), "=");
    string var_name = *prev(assignItr);
    Variable variable(var_name);
    AssignStatement stmt = AssignStatement(index, &variable);
    vector<string>::iterator endItr = find(content.begin(), content.end(), ";");
    vector<string> exprLst(next(assignItr), endItr);
    stmt.setExprLst(exprLst);
    return stmt;
}
