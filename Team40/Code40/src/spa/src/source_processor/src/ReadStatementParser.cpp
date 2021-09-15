#include "source_processor/ReadStatementParser.h"
#include "common/model/Statement.h"
#include <algorithm>
#include <string>
#include <vector>

ReadStatementParser::ReadStatementParser(vector<string> content, int index)
    : content(content), index(index) {
    auto stmt = new Statement(index, StatementType::READ);
};

Statement *ReadStatementParser::parseReadStatement() {
    vector<string>::iterator readItr =
        find(content.begin(), content.end(), "read");
    string var_name = *next(readItr);
    auto var = new Variable(var_name);
    stmt->setVariable(var);
    return stmt;
}
