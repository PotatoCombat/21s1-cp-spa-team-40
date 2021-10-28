#include "source_processor/parser/CallStatementParser.h"
#include <algorithm>

CallStatementParser::CallStatementParser(vector<string> content, int index)
    : EntityParser(content, index) {
    stmt = new Statement(index, StatementType::CALL);
};

Statement *CallStatementParser::parseEntity() {
    vector<string>::iterator callItr = find(content.begin(), content.end(), Tokens::KEYWORD_CALL);
    if (next(callItr) == content.end()) {
        throw invalid_argument("invalid call statement");
    }
    string proc_name = *next(callItr);
    if (!isValidName(proc_name)) {
        throw invalid_argument("invalid procedure name");
    }
    // call: 'call' proc_name Tokens::CHAR_SEMICOLON
    if (next(next(callItr)) == content.end()) {
        throw invalid_argument("invalid call statement");
    }
    if (*next(next(callItr)) != Tokens::SYMBOL_SEMICOLON) {
        throw invalid_argument("invalid call statement");
    }
    stmt->setProcName(proc_name);
    return stmt;
}
