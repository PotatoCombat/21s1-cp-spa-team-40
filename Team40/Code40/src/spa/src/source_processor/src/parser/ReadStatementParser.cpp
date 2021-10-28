#include "source_processor/parser/ReadStatementParser.h"
#include <algorithm>

ReadStatementParser::ReadStatementParser(vector<string> content, int index)
    : EntityParser(content, index) {
    stmt = new Statement(index, StatementType::READ);
};

Statement *ReadStatementParser::parseEntity() {
    vector<string>::iterator readItr = find(content.begin(), content.end(), "read");
    if (next(readItr) == content.end()) {
        throw invalid_argument("invalid read statement");
    }
    string var_name = *next(readItr);
    if (!isValidName(var_name)) {
        throw invalid_argument("invalid variable name");
    }
    // read: 'read' var_name';'
    if (next(next(readItr)) == content.end()) {
        throw invalid_argument("invalid read statement");
    }
    if (*next(next(readItr)) != ";") {
        throw invalid_argument("invalid read statement");
    }
    auto var = new Variable(var_name);
    stmt->setVariable(var);
    return stmt;
}
