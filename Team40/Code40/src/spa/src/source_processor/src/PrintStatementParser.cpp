#include "source_processor/PrintStatementParser.h"
#include "common/model/statement/PrintStatement.h"
#include <algorithm>

PrintStatementParser::PrintStatementParser(vector<string> content, int index)
    : StatementParser(content, index, {}, -1){};

Statement PrintStatementParser::parsePrintStatement() {
    vector<string>::iterator printItr =
        find(content.begin(), content.end(), "print");
    string var_name = *next(printItr);
    Variable variable(var_name);
    PrintStatement stmt = PrintStatement(index, variable);
    return stmt;
}
