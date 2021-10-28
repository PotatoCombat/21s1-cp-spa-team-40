#include "source_processor/parser/AssignStatementParser.h"
#include "source_processor/parser/ExpressionParser.h"
#include <algorithm>

AssignStatementParser::AssignStatementParser(vector<string> content, int index)
    : EntityParser(content, index) {
    stmt = new Statement(index, StatementType::ASSIGN);
};

Statement *AssignStatementParser::parseEntity() {
    vector<string>::iterator assignItr = find(content.begin(), content.end(), "=");
    if (assignItr == content.begin()) {
        throw invalid_argument("invalid assign statement");
    }
    string var_name = *prev(assignItr);
    if (!isValidName(var_name)) {
        throw invalid_argument("invalid variable name");
    }
    auto variable = new Variable(var_name);
    stmt->setVariable(variable);

    vector<string>::iterator endItr = find(content.begin(), content.end(), ";");
    if (endItr == content.end())
        throw invalid_argument("invalid assign statement");
    if (next(assignItr) == content.end()) {
        throw invalid_argument("invalid assign statement");
    }
    ExpressionParser exprParser(vector<string>(next(assignItr), endItr), stmt);
    vector<string> exprLst = exprParser.parseExpression();
    stmt->setExpressionLst(exprLst);
    return stmt;
}
