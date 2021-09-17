#pragma once
#include "common/model/Statement.h"
#include "source_processor/parser/Line.h"
#include <string>
#include <vector>

using namespace std;

class StatementParser {
private:
    vector<string> content;

public:
    StatementParser(vector<string> content);
    Statement *parseStatement(); // TODO: Remove unused method
    bool isReadStmt();
    bool isPrintStmt();
    bool isCallStmt();
    bool isWhileStmt();
    bool isAssignStmt();
    bool isIfStmt();
};
