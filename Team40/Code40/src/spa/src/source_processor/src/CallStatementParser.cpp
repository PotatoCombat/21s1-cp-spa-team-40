#include "source_processor/CallStatementParser.h"
#include <algorithm>

CallStatementParser::CallStatementParser(vector<string> content, int index)
    : content(content), index(index) {
    stmt = new Statement(index, StatementType::CALL);
};

Statement *CallStatementParser::parseCallStatement() {
    vector<string>::iterator callItr =
        find(content.begin(), content.end(), "call");
    string proc_name = *next(callItr);
    stmt->setProcName(proc_name);
    return stmt;
}
