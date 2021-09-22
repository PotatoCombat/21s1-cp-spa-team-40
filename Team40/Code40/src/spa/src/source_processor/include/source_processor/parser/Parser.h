#pragma once
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Statement.h"
#include "source_processor/parser/Line.h"
#include "source_processor/parser/ProcedureParser.h"
#include "source_processor/parser/StatementParser.h"
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

class Parser {
public:
    Program parseProgram(vector<Line> programLines);
    bool isAssignStmt(vector<string> content);
    bool isProc(vector<string> content);
    bool isStmt(vector<string> content);
    void checkAndAddValidProcedure(Procedure *currProc, Program &program);
    void checkValidProgram(Program program);
};
