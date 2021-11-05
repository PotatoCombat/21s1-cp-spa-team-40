#pragma once

#include "common/model/Statement.h"

#include "source_processor/parser/EntityParser.h"
#include "source_processor/parser/Line.h"

using namespace std;

class IfStatementParser : public EntityParser<Statement> {
private:
    vector<Line> programLines;

public:
    IfStatementParser(vector<string> content, int index, vector<Line> programLines);
    Statement *parseEntity(int &programIndex);
    void parseChildStmts(int &programIndex);
};
