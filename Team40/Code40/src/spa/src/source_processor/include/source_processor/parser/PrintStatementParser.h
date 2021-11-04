#pragma once

#include "common/model/Statement.h"

#include "source_processor/parser/EntityParser.h"

using namespace std;

class PrintStatementParser : public EntityParser<Statement> {
public:
    PrintStatementParser(vector<string> content, int index);
    Statement *parseEntity();
};
