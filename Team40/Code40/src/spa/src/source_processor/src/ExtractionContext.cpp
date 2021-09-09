#include "source_processor/ExtractionContext.h"

ExtractionContext &ExtractionContext::getInstance() {
    static ExtractionContext instance;
    return instance;
}

optional<ProcIndex> ExtractionContext::getCurrentProc() { return currentProc; }

void ExtractionContext::setCurrentProc(ProcIndex procIndex) {
    currentProc = procIndex;
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

optional<StmtIndex> ExtractionContext::getCurrentStatement() {
    return currentStatement;
}

void ExtractionContext::setCurrentStatement(StmtIndex stmtIndex) {
    currentStatement = stmtIndex;
}

optional<StmtIndex> ExtractionContext::getModifyingStatement() {
    return modifyingStatement;
}

void ExtractionContext::setModifyingStatement(StmtIndex stmtIndex) {
    modifyingStatement = stmtIndex;
}

void ExtractionContext::unsetModifyingStatement() { modifyingStatement = NULL; }

optional<StmtIndex> ExtractionContext::getUsingStatement() {
    return usingStatement;
}

void ExtractionContext::setUsingStatement(StmtIndex stmtIndex) {
    usingStatement = stmtIndex;
}

void ExtractionContext::unsetUsingStatement() { usingStatement = NULL; }
