#pragma once
#include "common/model/Statement.h"
#include "source_processor/parser/EntityParser.h"
using namespace std;

class ReadStatementParser : public EntityParser {
private:
    Statement *stmt;

public:
    ReadStatementParser(vector<string> content, int index);
    Statement *parseEntity();
};
