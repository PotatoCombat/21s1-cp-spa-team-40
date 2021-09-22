#pragma once
#include "common/model/Statement.h"
#include "source_processor/parser/AssignStatementParser.h"
#include "source_processor/parser/CallStatementParser.h"
#include "source_processor/parser/IfStatementParser.h"
#include "source_processor/parser/Line.h"
#include "source_processor/parser/PrintStatementParser.h"
#include "source_processor/parser/ReadStatementParser.h"
#include "source_processor/parser/WhileStatementParser.h"
#include <string>
#include <vector>

using namespace std;

class StatementParser {
private:
    vector<string> content;
    int index;
    vector<Line> programLines;
    int &programIndex;

public:
    StatementParser(vector<string> content, int index,
                    vector<Line> programLines, int &programIndex);
    Statement *parseStatement();
    bool isReadStmt();
    bool isPrintStmt();
    bool isCallStmt();
    bool isWhileStmt();
    bool isAssignStmt();
    bool isIfStmt();
};
