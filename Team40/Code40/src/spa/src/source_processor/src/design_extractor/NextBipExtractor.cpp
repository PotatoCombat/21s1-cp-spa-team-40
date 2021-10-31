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
    StmtIndex firstExecutedStmtIndex =
        ExtractionContext::getInstance().getFirstExecutedStatement(
            procedure->getName());
    Statement *firstExecutedStatement =
        pkb->getStmtByIndex(firstExecutedStmtIndex);
    extractStatement(firstExecutedStatement);
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

void NextBipExtractor::extractCallStatement(Statement *statement) {
    // Extract NextBip(statement, s1) where s1 is the first statement of called
    // proc
    ProcName calledProcName = statement->getProcName();
    StmtIndex calledProcFirstStmtIndex =
        ExtractionContext::getInstance().getFirstExecutedStatement(
            calledProcName);
    Statement *calledProcFirstStmt =
        pkb->getStmtByIndex(calledProcFirstStmtIndex);
    pkb->insertNextBip(statement, calledProcFirstStmt);

    // Extract NextBip(s2, s3) where s2 is last-executable non-call
    // statement of calledProc and Next(statement, s3) holds.
    set<ProgLineIndex> nextStmtIndices =
        pkb->getNextLines(statement->getIndex());
    if (nextStmtIndices.empty()) {
        return;
    }
    if (nextStmtIndices.size() > 1) {
        throw runtime_error("Encountered a call statement with >1 next "
                            "statements (syntactically impossible).");
    }
    Statement *nextStatement = pkb->getStmtByIndex(*nextStmtIndices.begin());
    // If s2 is a call statement, recursively find non-call last-executable
    // statement.
    set<StmtIndex> calledProcLastStmtIndices =
        ExtractionContext::getInstance().getLastExecutedStatements(
            calledProcName);
    for (auto calledProcLastStmtIndex : calledProcLastStmtIndices) {
        Statement *calledProcLastStmt =
            pkb->getStmtByIndex(calledProcLastStmtIndex);
        pkb->insertNextBip(calledProcLastStmt, nextStatement);
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
