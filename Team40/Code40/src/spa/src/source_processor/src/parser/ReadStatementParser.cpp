#include "source_processor/parser/ReadStatementParser.h"
#include <algorithm>

ReadStatementParser::ReadStatementParser(vector<string> content, int index)
    : EntityParser(content, index) {
    entity = new Statement(index, StatementType::READ);
};

/**
 * Parses a tokenized string identified to be a read statement into a
 * Statement object of type READ.
 * @return Statement object of type READ.
 */
Statement *ReadStatementParser::parseEntity() {
    vector<string>::iterator readItr = find(content.begin(), content.end(), Tokens::KEYWORD_READ);
    if (next(readItr) == content.end()) {
        throw invalid_argument("invalid read statement");
    }
    string var_name = *next(readItr);
    if (!isValidName(var_name)) {
        throw invalid_argument("invalid variable name");
    }
    // read: 'read' var_nameTokens::CHAR_SEMICOLON
    if (next(next(readItr)) == content.end()) {
        throw invalid_argument("invalid read statement");
    }
    if (*next(next(readItr)) != Tokens::SYMBOL_SEMICOLON) {
        throw invalid_argument("invalid read statement");
    }
    Variable *var = new Variable(var_name);
    entity->setVariable(var);
    return entity;
}
