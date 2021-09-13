#include "source_processor/AssignStatementParser.h"
#include "common/model/ConstantValue.h"
#include "common/model/Variable.h"
#include <algorithm>

AssignStatementParser::AssignStatementParser(vector<string> content, int index)
    : StatementParser(content, index, {}, -1){};

Statement AssignStatementParser::parseAssignStatement() {
    vector<string>::iterator assignItr =
        find(content.begin(), content.end(), "=");
    string var_name = *prev(assignItr);
    Statement stmt = Statement(index, StatementType::ASSIGN);
    Variable variable(var_name);
    stmt.setVariable(&variable);
    vector<string>::iterator endItr = find(content.begin(), content.end(), ";");
    vector<string> exprLst(next(assignItr), endItr);
    stmt.setExpressionLst(exprLst);
    parseExpression(exprLst, stmt);
    return stmt;
}

void AssignStatementParser::parseExpression(vector<string> exprLst,
                                            Statement &stmt) {
    for (int i = 0; i < exprLst.size(); i++) {
        string currString = exprLst[i];
        if (isInteger(currString)) {
            ConstantValue constVal(stoi(currString));
            stmt.addExpressionConst(&constVal);
        } else if (isName(currString)) {
            Variable var(currString);
            stmt.addExpressionVar(&var);
        }
    }
}
