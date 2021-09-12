#pragma once
#include "source_processor/StatementParser.h"
using namespace std;

class CallStatementParser : StatementParser {
public:
    CallStatementParser(vector<string> content, int index);
    Statement parseCallStatement();
};
