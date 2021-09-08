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
    // 1. Store into PKB
    pkb.insertProc(procedure);
    // 2. Iterate (BFS)
    for (Statement statement : procedure->getStmtLst()) {
        // handleStatement()
    }
    // 3. Tie-up
}
