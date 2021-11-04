#include "source_processor/parser/AssignStatementParser.h"
#include "source_processor/parser/ExpressionParser.h"
#include <algorithm>

AssignStatementParser::AssignStatementParser(vector<string> content, int index)
    : EntityParser(content, index) {
    entity = new Statement(index, StatementType::ASSIGN);
};

/**
 * Parses a tokenized string identified to be an assign statement into a
 * Statement object of type ASSIGN.
 * @return Statement object of type ASSIGN.
 */
Statement *AssignStatementParser::parseEntity() {
    vector<string>::iterator assignItr =
        find(content.begin(), content.end(), Tokens::SYMBOL_ASSIGN);
    if (assignItr == content.begin()) {
        throw invalid_argument("invalid assign statement");
    }
    string var_name = *prev(assignItr);
    if (!isValidName(var_name)) {
        throw invalid_argument("invalid variable name");
    }
    Variable *variable = new Variable(var_name);
    entity->setVariable(variable);

    vector<string>::iterator endItr =
        find(content.begin(), content.end(), Tokens::SYMBOL_SEMICOLON);
    if (endItr == content.end())
        throw invalid_argument("invalid assign statement");
    if (next(assignItr) == content.end()) {
        throw invalid_argument("invalid assign statement");
    }
    ExpressionParser exprParser(vector<string>(next(assignItr), endItr), entity);
    vector<string> exprLst = exprParser.parseExpression();
    entity->setExpressionLst(exprLst);
    return entity;
}
