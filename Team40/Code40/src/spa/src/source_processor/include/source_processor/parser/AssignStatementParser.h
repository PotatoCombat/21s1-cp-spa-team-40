#pragma once
#include "common/model/Statement.h"
#include "source_processor/parser/EntityParser.h"
using namespace std;

class AssignStatementParser : public EntityParser<Statement> {
public:
    AssignStatementParser(vector<string> content, int index);
    Statement *parseEntity();
};
