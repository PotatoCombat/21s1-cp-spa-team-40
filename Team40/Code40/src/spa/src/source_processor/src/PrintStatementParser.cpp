#include "source_processor/PrintStatementParser.h"
#include <algorithm>

PrintStatementParser::PrintStatementParser(vector<string> content, int index)
    : StatementParser(content, index, {}, -1){};

Statement PrintStatementParser::parsePrintStatement() {
    vector<string>::iterator printItr =
        find(content.begin(), content.end(), "print");
    string var_name = *next(printItr);
    Statement stmt = Statement(index, StatementType::PRINT);
    stmt.setVariable(Variable(var_name));
    return stmt;
}
