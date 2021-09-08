#include "source_processor/DesignExtractor.h"

using namespace std;

#include "pkb/PKB.h"

DesignExtractor::DesignExtractor(PKB pkb) : pkb(pkb) {}

void DesignExtractor::handleProgram(Program program) {
    PKB pkb = PKB();
    for (Procedure proc : program.getProcLst()) {
        handleProcedure(&proc);
    }
}

void DesignExtractor::handleProcedure(Procedure *procedure) {
    pkb.insertProc(procedure);
    for (Statement statement : procedure->getStmtLst()) {
        handleStatement(&statement);
    }
    // 3. Tie-up
}

void DesignExtractor::handleStatement(Statement *statement) {
    pkb.insertStmt(statement);
}
