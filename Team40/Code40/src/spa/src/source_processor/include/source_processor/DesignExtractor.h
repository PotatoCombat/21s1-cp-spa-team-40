#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/statement/AssignStatement.h"
#include "common/model/statement/Statement.h"
#include "pkb/PKB.h"
#include "source_processor/ExtractionContext.h"

class DesignExtractor {
    PKB pkb;

public:
    DesignExtractor(PKB pkb);
    void handleProgram(Program program);
    ProcIndex handleProcedure(Procedure *program);
    StmtIndex handleStatement(Statement *statement);
    StmtIndex handleAssignStatement(Statement *assignStatement);
    StmtIndex handleCallStatement(Statement *callStatement);
    StmtIndex handleIfStatement(Statement *ifStatement);
    void handleContextualRelationships(StmtIndex stmtIndex);
};