#include "source_processor/ExtractionContext.h"

ExtractionContext &ExtractionContext::getInstance() {
    static ExtractionContext instance;
    return instance;
}

optional<ProcIndex> ExtractionContext::getCurrentProc() { return currentProc; }

void ExtractionContext::setCurrentProc(ProcIndex procIndex) {
    if (currentProc.has_value()) {
        throw runtime_error(
            "Trying to overwrite an existing Procedure's context");
    }
    currentProc = procIndex;
}

void ExtractionContext::unsetCurrentProc() { currentProc = NULL; }

void ExtractionContext::setPrevStatement(StmtIndex stmtIndex) {
    if (prevStatement.has_value()) {
        throw runtime_error(
            "Trying to overwrite an existing Statement's context");
    }
    prevStatement = stmtIndex;
}

void ExtractionContext::unsetPrevStatement() { prevStatement = NULL; }

optional<StmtIndex> ExtractionContext::getCurrentStatement() {
    return currentStatement;
}

void ExtractionContext::setCurrentStatement(StmtIndex stmtIndex) {
    if (currentStatement.has_value()) {
        throw runtime_error(
            "Trying to overwrite an existing Statement's context");
    }
    currentStatement = stmtIndex;
}

void ExtractionContext::unsetCurrentStatement() { currentStatement = NULL; }

optional<StmtIndex> ExtractionContext::getParentStatement() {
    if (parentStatements.empty()) {
        return NULL;
    }
    return parentStatements.back();
}

void ExtractionContext::setParentStatement(StmtIndex stmtIndex) {
    parentStatements.push_back(stmtIndex);
}

void ExtractionContext::unsetParentStatement() {
    if (parentStatements.empty()) {
        throw runtime_error("Trying to pop empty stack.");
    }
    parentStatements.pop_back();
}

optional<StmtIndex> ExtractionContext::getPrevStatement() {
    return prevStatement;
}

optional<StmtIndex> ExtractionContext::getModifyingStatement() {
    if (modifyingStatements.empty()) {
        return NULL;
    }
    return modifyingStatements.back();
}

void ExtractionContext::setModifyingStatement(StmtIndex stmtIndex) {
    modifyingStatements.push_back(stmtIndex);
}

void ExtractionContext::unsetModifyingStatement() {
    if (modifyingStatements.empty()) {
        throw runtime_error("Trying to pop empty stack.");
    }
    modifyingStatements.pop_back();
}

optional<StmtIndex> ExtractionContext::getUsingStatement() {
    if (usingStatements.empty()) {
        return NULL;
    }
    return usingStatements.back();
}

void ExtractionContext::setUsingStatement(StmtIndex stmtIndex) {
    usingStatements.push_back(stmtIndex);
}

void ExtractionContext::unsetUsingStatement() {
    if (usingStatements.empty()) {
        throw runtime_error("Trying to pop empty stack.");
    }
    usingStatements.pop_back();
}
