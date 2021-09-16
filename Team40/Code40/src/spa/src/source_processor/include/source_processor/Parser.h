#pragma once
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Statement.h"
#include "source_processor/AssignStatementParser.h"
#include "source_processor/CallStatementParser.h"
#include "source_processor/IfStatementParser.h"
#include "source_processor/Line.h"
#include "source_processor/PrintStatementParser.h"
#include "source_processor/ProcedureParser.h"
#include "source_processor/ReadStatementParser.h"
#include "source_processor/StatementParser.h"
#include "source_processor/WhileStatementParser.h"
#include <algorithm>
#include <string>
#include <vector>
using namespace std;

class Parser {
public:
    Program parseProgram(vector<Line> programLines, Program &program);
    Statement *parseStatement(vector<string> content, int index,
                              vector<Line> programLines, int &programIndex);
    void checkAndAddValidProcedure(Procedure *currProc, Program &program);
    void Parser::checkValidProgram(Program program);
};
