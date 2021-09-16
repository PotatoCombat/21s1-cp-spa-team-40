#include "source_processor/parser/CallStatementParser.h"
#include <algorithm>

CallStatementParser::CallStatementParser(vector<string> content, int index)
    : content(content), index(index) {
    stmt = new Statement(index, StatementType::CALL);
};

Statement *CallStatementParser::parseCallStatement() {
    vector<string>::iterator callItr =
        find(content.begin(), content.end(), "call");
    string proc_name = *next(callItr);
    if (!isValidName(proc_name)) {
        throw invalid_argument("invalid procedure name");
    }
    // call: 'call' proc_name ';'
    if (*next(next(callItr)) != ";") {
        throw invalid_argument("invalid call statement");
    }
    stmt->setProcName(proc_name);
    return stmt;
}

bool CallStatementParser::isValidName(string input) {
    // NAME: LETTER (LETTER | DIGIT)*
    // procedure names and variables are strings of letters, and digits,
    // starting with a letter
    if (!isalpha(input.at(0))) {
        return false;
    }
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isalnum(c)); }) == input.end();
}
