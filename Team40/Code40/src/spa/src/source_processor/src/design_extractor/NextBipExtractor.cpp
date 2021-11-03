#include "source_processor/design_extractor/NextBipExtractor.h"

NextBipExtractor::NextBipExtractor(PKB *pkb) : pkb(pkb) {
    nextTerminalIndex = STARTING_TERMINAL_INDEX;
}

void NextBipExtractor::extract(Program *program) {
    ExtractionContext::getInstance().resetTransientContexts();

    // Extract procedures in topologically reverse order of dependencies
    vector<ProcName> sortedProcNames =
        ExtractionContext::getInstance().getTopologicallySortedProcNames();
    for (auto it = sortedProcNames.rbegin(); it != sortedProcNames.rend();
         it++) {
        Procedure *proc = pkb->getProcByName(*it);
        extractProcedure(proc);
    }
}

void NextBipExtractor::extractProcedure(Procedure *procedure) {
    ExtractionContext::getInstance().setCurrentProcedure(procedure);
    // Create terminal stmt (equivalent of dummy node in CFG)
    Statement *terminalStmt =
        new Statement(getNextTerminalIndex(), StatementType::TERMINAL);
    terminalStmtsMap[procedure->getName()] = terminalStmt;

    // Extract statements recursively
    Statement *firstExecutableStmt =
        pkb->getProcByName(procedure->getName())->getStmtLst().front();
    extractStatement(firstExecutableStmt);

    ExtractionContext::getInstance().unsetCurrentProcedure(procedure);
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
    auto terminalStmt = terminalStmtsMap[calledProcName];
    auto nextStatement = getStatementAfterCallStatement(callStmtIndex);

    extractCallStatementNextBip(callStatement, calledProcFirstExecutableStmt,
                                terminalStmt, nextStatement);
    if (nextStatement.has_value()) {
        extractStatement(nextStatement.value());
    } else {
        extractTerminalNextBip(callStatement);
    }
}

void NextBipExtractor::extractNonCallStatement(Statement *statement) {
    if (isLastExecutableStmt(statement)) {
        extractTerminalNextBip(statement);
    }
    set<ProgLineIndex> nextStatements =
        pkb->getNextLines(statement->getIndex());
    for (ProgLineIndex nextStatementIndex : nextStatements) {
        Statement *nextStatement = pkb->getStmtByIndex(nextStatementIndex);
        pkb->insertNextBip(statement, nextStatement);
        extractStatement(nextStatement);
    }
}

void NextBipExtractor::extractCallStatementNextBip(
    Statement *branchInFrom, Statement *branchInTo, Statement *branchBackFrom,
    optional<Statement *> branchBackTo) {
    pkb->insertBranchIn(branchInFrom, branchInTo);
    pkb->insertNextBip(branchInFrom, branchInTo);
    if (branchBackTo.has_value()) {
        pkb->insertNextBip(branchBackFrom, branchBackTo.value());
        pkb->insertBranchBack(branchBackFrom, branchBackTo.value());
    } else {
        ProcName currentProcName = getCurrentProcName();
        pkb->insertNextBip(branchBackFrom, terminalStmtsMap[currentProcName]);
        pkb->insertBranchBack(branchBackFrom,
                              terminalStmtsMap[currentProcName]);
    }
}

Statement *NextBipExtractor::getFirstExecutableStmt(const ProcName &procName) {
    StmtIndex calledProcFirstStmtIndex =
        ExtractionContext::getInstance().getFirstExecutableStatement(procName);
    return pkb->getStmtByIndex(calledProcFirstStmtIndex);
}

optional<Statement *>
NextBipExtractor::getStatementAfterCallStatement(StmtIndex callStmtIndex) {
    set<ProgLineIndex> nextStmtIndices = pkb->getNextLines(callStmtIndex);
    if (nextStmtIndices.empty()) {
        return nullopt;
    }
    if (nextStmtIndices.size() > 1) {
        throw runtime_error(
            "Encountered a call statement with more than one next "
            "statement (syntactically impossible).");
    }
    return pkb->getStmtByIndex(*nextStmtIndices.begin());
}

ProcName NextBipExtractor::getCurrentProcName() {
    auto currentProc = ExtractionContext::getInstance().getCurrentProcedure();
    if (!currentProc.has_value()) {
        throw runtime_error("Current procedure not set.");
    }
    return currentProc.value()->getName();
}

void NextBipExtractor::extractTerminalNextBip(Statement *statement) {
    ProcName currentProcName = getCurrentProcName();
    if (!terminalStmtsMap.count(currentProcName)) {
        throw runtime_error("Current procedure terminal stmt not set.");
    }

    pkb->insertNextBip(statement, terminalStmtsMap[currentProcName]);
}

bool NextBipExtractor::isLastExecutableStmt(Statement *statement) {
    set<ProgLineIndex> nextStatements =
        pkb->getNextLines(statement->getIndex());
    return nextStatements.empty() ||
           (statement->getStatementType() == StatementType::WHILE &&
            nextStatements.size() < 2);
}