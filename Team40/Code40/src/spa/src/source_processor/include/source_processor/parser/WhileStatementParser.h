#pragma once
#include "common/model/Statement.h"
#include "source_processor/parser/EntityParser.h"
#include "source_processor/parser/Line.h"
using namespace std;

class WhileStatementParser : public EntityParser {
private:
    Statement *stmt;
    vector<Line> programLines;

public:
    WhileStatementParser(vector<string> content, int index, vector<Line> programLines);
    Statement *parseEntity(int &programIndex);
    void parseChildStatements(int &programIndex);
};