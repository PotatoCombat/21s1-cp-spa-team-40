#include "source_processor/design_extractor/NextBipExtractor.h"

NextBipExtractor::NextBipExtractor(PKB *pkb) : pkb(pkb) {}

void NextBipExtractor::extract(Program *program) {
    // Extract procedures in topological order of dependencies
    unordered_map<ProcName, Procedure *> procMap;
    for (Procedure *procedure : program->getProcLst()) {
        procMap[procedure->getName()] = procedure;
    }
    vector<ProcName> sortedProcNames =
        ExtractionContext::getInstance().getTopologicallySortedProcNames();
    for (const auto &procName : sortedProcNames) {
        extractProcedure(procMap[procName]);
    }
}

void NextBipExtractor::extractProcedure(Procedure *procedure) {
    StmtIndex firstExecutableStmtIndex =
        ExtractionContext::getInstance().getFirstExecutableStatement(
            procedure->getName());
    Statement *firstExecutableStatement =
        pkb->getStmtByIndex(firstExecutableStmtIndex);
    extractStatement(firstExecutableStatement);
}

void NextBipExtractor::extractStatement(Statement *statement) {
    // If previously extracted, do nothing
    ProgLineIndex index = statement->getIndex();
    if (visited.count(index)) {
        return;
    }
    visited.insert(index);
    if (statement->getStatementType() == StatementType::CALL) {
        extractCallStatement(statement);
    } else {
        extractNonCallStatement(statement);
    }
}

void NextBipExtractor::extractCallStatement(Statement *callStatement) {
    ProcName calledProcName = callStatement->getProcName();
    StmtIndex callStmtIndex = callStatement->getIndex();

    auto calledProcFirstExecutableStmt = getFirstExecutableStmt(calledProcName);
    auto calledProcLastExecutableStmts = getLastExecutableStmts(calledProcName);
    auto nextStatement = getStatementAfterCallStatement(callStmtIndex);

    extractNextBip(callStatement, calledProcFirstExecutableStmt,
                   calledProcLastExecutableStmts, nextStatement);
    if (nextStatement.has_value()) {
        extractStatement(nextStatement.value());
    }
}

void NextBipExtractor::extractNonCallStatement(Statement *statement) {
    set<ProgLineIndex> nextStatements =
        pkb->getNextLines(statement->getIndex());
    if (nextStatements.empty()) { // Reached end of procedure
        if (!callbacks.empty()) {
            nextStatements = callbacks.back();
            callbacks.pop_back();
        }
    }
    for (ProgLineIndex nextStatementIndex : nextStatements) {
        Statement *nextStatement = pkb->getStmtByIndex(nextStatementIndex);
        pkb->insertNextBip(statement, nextStatement);
        extractStatement(nextStatement);
    }
}

void NextBipExtractor::extractNextBip(Statement *branchInFrom,
                                      Statement *branchInTo,
                                      set<Statement *> branchBackFroms,
                                      optional<Statement *> branchBackTo) {
    for (auto branchBackFrom : branchBackFroms) {
        pkb->insertBranchIn(branchInFrom, branchInTo);
        pkb->insertNextBip(branchInFrom, branchInTo);
        if (branchBackTo.has_value()) {
            pkb->insertNextBip(branchBackFrom, branchBackTo.value());
            pkb->insertBranchBack(branchBackFrom, branchBackTo.value());
        }
    }
}

Statement *NextBipExtractor::getFirstExecutableStmt(ProcName procName) {
    StmtIndex calledProcFirstStmtIndex =
        ExtractionContext::getInstance().getFirstExecutableStatement(procName);
    return pkb->getStmtByIndex(calledProcFirstStmtIndex);
}

set<Statement *> NextBipExtractor::getLastExecutableStmts(ProcName procName) {
    set<StmtIndex> procLastStmtIndices =
        ExtractionContext::getInstance().getLastExecutableStatements(procName);
    set<Statement *> procLastStmts;
    for (auto procLastStmtIndex : procLastStmtIndices) {
        Statement *procLastStmt = pkb->getStmtByIndex(procLastStmtIndex);
        procLastStmts.insert(procLastStmt);
    }
    return procLastStmts;
}

optional<Statement *>
NextBipExtractor::getStatementAfterCallStatement(StmtIndex callStmtIndex) {
    set<ProgLineIndex> nextStmtIndices = pkb->getNextLines(callStmtIndex);
    if (nextStmtIndices.empty()) {
        return nullopt;
    }
    if (nextStmtIndices.size() > 1) {
        throw runtime_error("Encountered a call statement with >1 next "
                            "statements (syntactically impossible).");
    }
    return pkb->getStmtByIndex(*nextStmtIndices.begin());
}