#include "source_processor/parser/CallStatementParser.h"
#include <algorithm>

CallStatementParser::CallStatementParser(vector<string> content, int index)
    : EntityParser(content, index) {
    entity = new Statement(index, StatementType::CALL);
};

/**
 * Parses a tokenized string identified to be a call statement into a
 * Statement object of type CALL.
 * @return Statement object of type CALL.
 */
Statement *CallStatementParser::parseEntity() {
    vector<string>::iterator callItr = find(content.begin(), content.end(), Tokens::KEYWORD_CALL);
    if (next(callItr) == content.end()) {
        throw invalid_argument("invalid call statement");
    }
    string proc_name = *next(callItr);
    if (!isValidName(proc_name)) {
        throw invalid_argument("invalid procedure name");
    }
    // call: 'call' proc_name ';'
    if (next(next(callItr)) == content.end()) {
        throw invalid_argument("invalid call statement");
    }
    if (*next(next(callItr)) != Tokens::SYMBOL_SEMICOLON) {
        throw invalid_argument("invalid call statement");
    }
    entity->setProcName(proc_name);
    return entity;
}
