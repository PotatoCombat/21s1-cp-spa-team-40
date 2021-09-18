#pragma once
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Statement.h"
#include "source_processor/parser/AssignStatementParser.h"
#include "source_processor/parser/CallStatementParser.h"
#include "source_processor/parser/IfStatementParser.h"
#include "source_processor/parser/Line.h"
#include "source_processor/parser/PrintStatementParser.h"
#include "source_processor/parser/ProcedureParser.h"
#include "source_processor/parser/ReadStatementParser.h"
#include "source_processor/parser/StatementParser.h"
#include "source_processor/parser/WhileStatementParser.h"
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

class Parser {
public:
    Program parseProgram(vector<Line> programLines, Program &program);
    bool isAssignStmt(vector<string> content);
    bool isProc(vector<string> content);
    bool isStmt(vector<string> content);

    Statement *parseStatement(vector<string> content, int index,
                              vector<Line> programLines, int &programIndex);
    void checkAndAddValidProcedure(Procedure *currProc, Program &program);
    void checkValidProgram(Program program);
};
