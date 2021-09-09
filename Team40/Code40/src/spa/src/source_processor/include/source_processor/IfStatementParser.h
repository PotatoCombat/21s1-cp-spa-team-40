#pragma once
#include "source_processor/StatementParser.h"
using namespace std;

class IfStatementParser : StatementParser {
public:
    IfStatementParser(vector<string> content, int index,
                      vector<Line> programLines, int programIndex);
    Statement parseIfStatement();
    bool hasTerminator(vector<string> inputLine);
};