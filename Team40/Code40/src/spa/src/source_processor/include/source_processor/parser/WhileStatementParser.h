#pragma once
#include "common/model/Statement.h"
#include "source_processor/parser/Line.h"
#include <string>
#include <vector>

using namespace std;

class WhileStatementParser {
private:
    Statement *stmt;
    vector<string> content;
    int index;
    vector<Line> programLines;

public:
    WhileStatementParser(vector<string> content, int index, vector<Line> programLines);
    Statement *parseWhileStatement(int &programIndex);
    void checkValidCondition(vector<string> condLst);
    void parseChildStatements(int &programIndex);
};