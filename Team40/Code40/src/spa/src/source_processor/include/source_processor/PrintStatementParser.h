#pragma once
#include "source_processor/StatementParser.h"
using namespace std;

class PrintStatementParser : StatementParser {
public:
    PrintStatementParser(vector<string> content, int index);
    Statement parsePrintStatement();
};
