#include "source_processor/parser/PrintStatementParser.h"
#include <algorithm>

PrintStatementParser::PrintStatementParser(vector<string> content, int index)
    : EntityParser(content, index) {
    entity = new Statement(index, StatementType::PRINT);
};

/**
 * Parses a tokenized string identified to be a print statement into a
 * Statement object of type PRINT.
 * @return Statement object of type PRINT.
 */
Statement *PrintStatementParser::parseEntity() {
    vector<string>::iterator printItr = find(content.begin(), content.end(), Tokens::KEYWORD_PRINT);
    if (next(printItr) == content.end()) {
        throw invalid_argument("invalid print statement");
    }
    string var_name = *next(printItr);
    if (!isValidName(var_name)) {
        throw invalid_argument("invalid variable name");
    }
    // print: 'print' var_nameTokens::CHAR_SEMICOLON
    if (next(next(printItr)) == content.end()) {
        throw invalid_argument("invalid print statement");
    }
    if (*next(next(printItr)) != Tokens::SYMBOL_SEMICOLON) {
        throw invalid_argument("invalid print statement");
    }
    Variable *variable = new Variable(var_name);
    entity->setVariable(variable);
    return entity;
}
