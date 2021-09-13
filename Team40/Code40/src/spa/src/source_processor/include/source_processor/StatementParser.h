#pragma once
#include "common/model/Statement.h"
#include "source_processor/Line.h"
#include <string>
#include <vector>

using namespace std;

class StatementParser {
protected:
    vector<string> content;
    int index;
    vector<Line> programLines;
    int programIndex;

public:
    StatementParser(vector<string> content, int index,
                    vector<Line> programLines, int programIndex);
    Statement parseStatement();

    bool isReadStmt(vector<string> inputLine);
    bool isPrintStmt(vector<string> inputLine);
    bool isCallStmt(vector<string> inputLine);
    bool isWhileStmt(vector<string> inputLine);
    bool isAssignStmt(vector<string> inputLine);
    bool isIfStmt(vector<string> inputLine);
    bool hasTerminator(vector<string> inputLine);
    bool isInteger(string input);
    bool isName(string input);
};
