#pragma once
#include "source_processor/StatementParser.h"
using namespace std;

class AssignStatementParser : StatementParser {
public:
    AssignStatementParser(vector<string> content, int index);
    Statement *parseAssignStatement();
    void parseExpression(vector<string> exprLst, Statement *statement);
};
