#pragma once
#include "source_processor/StatementParser.h"

using namespace std;

class IfStatementParser : StatementParser {
public:
    IfStatementParser(vector<string> content, int index,
                      vector<Line> programLines, int &programIndex);
    Statement *parseIfStatement(int &programIndex);
    void parseChildStatements(Statement *stmt);
    void parseExpression(vector<string> exprLst, Statement *statement);
};