#include "source_processor/ReadStatementParser.h"
#include "common/model/statement/ReadStatement.h"
#include <algorithm>

ReadStatementParser::ReadStatementParser(vector<string> content, int index)
    : StatementParser(content, index, {}, -1){};

Statement ReadStatementParser::parseReadStatement() {
    vector<string>::iterator readItr =
        find(content.begin(), content.end(), "read");
    string var_name = *next(readItr);
    Variable variable(var_name);
    ReadStatement stmt = ReadStatement(index, &variable);
    return stmt;
}
