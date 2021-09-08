#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/statement/AssignStatement.h"
#include "common/model/statement/Statement.h"
#include "pkb/PKB.h"

class DesignExtractor {
    PKB pkb;

public:
    DesignExtractor(PKB pkb);
    void handleProgram(Program program);
    void handleProcedure(Procedure *program);
    void handleStatement(Statement *statement);
    void handleAssignStatement(Statement *assignStatement);
};