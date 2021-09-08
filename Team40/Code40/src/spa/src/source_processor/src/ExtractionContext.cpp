#include "source_processor/ExtractionContext.h"

ExtractionContext &ExtractionContext::getInstance() {
    static ExtractionContext instance;
    return instance;
}

optional<StmtIndex> ExtractionContext::getParentStatement() {
    return parentStatement;
}

void ExtractionContext::setParentStatement(StmtIndex stmtIndex) {
    parentStatement = stmtIndex;
}

optional<StmtIndex> ExtractionContext::getPrevStatement() {
    return prevStatement;
}

void ExtractionContext::setPrevStatement(StmtIndex stmtIndex) {
    prevStatement = stmtIndex;
}

optional<ProcIndex> ExtractionContext::getCurrentProc() { return currentProc; }

void ExtractionContext::setCurrentProc(ProcIndex procIndex) {
    currentProc = procIndex;
}
