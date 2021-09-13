#include "source_processor/ReadStatementParser.h"
#include <algorithm>

ReadStatementParser::ReadStatementParser(vector<string> content, int index)
    : StatementParser(content, index, {}, -1){};

Statement ReadStatementParser::parseReadStatement() {
    vector<string>::iterator readItr =
        find(content.begin(), content.end(), "read");
    string var_name = *next(readItr);
    Statement stmt = Statement(index, StatementType::READ);
    stmt.setVariable(Variable(var_name));
    return stmt;
}
