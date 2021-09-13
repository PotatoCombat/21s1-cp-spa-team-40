#pragma once
#include "source_processor/StatementParser.h"
using namespace std;

class WhileStatementParser : StatementParser {
public:
    WhileStatementParser(vector<string> content, int index,
                         vector<Line> programLines, int &programIndex);
    Statement *parseWhileStatement(int &programIndex);
    void parseChildStatements(Statement *stmt);
    void parseExpression(vector<string> exprLst, Statement *statement);
};