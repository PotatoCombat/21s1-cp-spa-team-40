#include "source_processor/ReadStatementParser.h"
#include <algorithm>

ReadStatementParser::ReadStatementParser(vector<string> content, int index)
    : content(content), index(index) {
    stmt = new Statement(index, StatementType::READ);
};

Statement *ReadStatementParser::parseReadStatement() {
    vector<string>::iterator readItr =
        find(content.begin(), content.end(), "read");
    string var_name = *next(readItr);
    if (!isValidName(var_name)) {
        throw("invalid variable name");
    }
    // read: 'read' var_name';'
    if (*next(next(readItr)) != ";") {
        throw("invalid read statement");
    }
    auto var = new Variable(var_name);
    stmt->setVariable(var);
    return stmt;
}

bool ReadStatementParser::isValidName(string input) {
    // NAME: LETTER (LETTER | DIGIT)*
    // procedure names and variables are strings of letters, and digits,
    // starting with a letter
    if (!isalpha(input.at(0))) {
        return false;
    }
    return find_if(input.begin(), input.end(),
                   [](char c) { return !(isalnum(c)); }) == input.end();
}
