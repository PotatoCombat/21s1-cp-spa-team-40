#include "source_processor/ReadStatementParser.h"
#include <algorithm>

ReadStatementParser::ReadStatementParser(vector<string> content, int index)
    : StatementParser(content, index, {}, -1){};

Statement* ReadStatementParser::parseReadStatement() {
    vector<string>::iterator readItr =
        find(content.begin(), content.end(), "read");
    string var_name = *next(readItr);
    auto stmt = new Statement(index, StatementType::READ);
    auto var = new Variable(var_name);
    stmt->setVariable(var);
    return stmt;
}
