#include "source_processor/CallStatementParser.h"
#include <algorithm>

CallStatementParser::CallStatementParser(vector<string> content, int index)
    : StatementParser(content, index, {}, -1){};

Statement *CallStatementParser::parseCallStatement() {
    vector<string>::iterator callItr =
        find(content.begin(), content.end(), "call");
    string proc_name = *next(callItr);
    auto stmt = new Statement(index, StatementType::CALL);
    stmt->setProcName(proc_name);
    return stmt;
}
