#include "source_processor/PrintStatementParser.h"
#include "common/model/statement/PrintStatement.h"
#include <algorithm>

PrintStatementParser::PrintStatementParser(vector<string> content, int index)
    : StatementParser(content, index, {}, -1){};

Statement PrintStatementParser::parsePrintStatement() {
    vector<string>::iterator printItr =
        find(content.begin(), content.end(), "print");
    string var_name = *next(printItr);
    PrintStatement stmt = PrintStatement(index, Variable(var_name));
    return stmt;
}
