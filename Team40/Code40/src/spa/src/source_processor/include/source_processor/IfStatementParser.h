#pragma once
#include "common/model/Statement.h"
#include "source_processor/Line.h"
#include <string>
#include <vector>

using namespace std;

class IfStatementParser {
private:
    Statement *stmt;
    vector<string> content;
    int index;
    vector<Line> programLines;
    int programIndex;

public:
    IfStatementParser(vector<string> content, int index,
                      vector<Line> programLines);
    Statement *parseIfStatement(int &programIndex);
    void parseChildStatements();
};