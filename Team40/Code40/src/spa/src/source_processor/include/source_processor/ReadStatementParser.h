#pragma once
#include "source_processor/StatementParser.h"
using namespace std;

class ReadStatementParser : StatementParser {
public:
    ReadStatementParser(vector<string> content, int index);
    Statement* parseReadStatement();
};
