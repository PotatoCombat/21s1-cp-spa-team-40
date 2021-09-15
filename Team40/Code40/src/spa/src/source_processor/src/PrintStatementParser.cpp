#include "source_processor/PrintStatementParser.h"
#include "common/model/Statement.h"
#include <algorithm>
#include <string>
#include <vector>

PrintStatementParser::PrintStatementParser(vector<string> content, int index)
    : content(content), index(index) {
    stmt = new Statement(index, StatementType::PRINT);
};

Statement *PrintStatementParser::parsePrintStatement() {
    vector<string>::iterator printItr =
        find(content.begin(), content.end(), "print");
    string var_name = *next(printItr);
    auto variable = new Variable(var_name);
    stmt->setVariable(variable);
    return stmt;
}
