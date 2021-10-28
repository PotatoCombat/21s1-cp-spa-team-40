#pragma once
#include "common/model/Statement.h"
#include "source_processor/parser/EntityParser.h"
using namespace std;

class CallStatementParser : public EntityParser {
private:
    Statement *stmt;

public:
    CallStatementParser(vector<string> content, int index);
    Statement *parseEntity();
};
