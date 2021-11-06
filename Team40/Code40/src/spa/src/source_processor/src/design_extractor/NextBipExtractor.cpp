#include "source_processor/design_extractor/NextBipExtractor.h"

NextBipExtractor::NextBipExtractor(PKB *pkb) : pkb(pkb) {
    nextTerminalIndex = NextBipExtractor::STARTING_TERMINAL_INDEX;
}

void NextBipExtractor::extract(Program *program) {
    ExtractionContext::getInstance().resetTransientContexts();

    // Extract procedures in topologically reverse order of dependencies
    vector<ProcName> sortedProcNames =
        ExtractionContext::getInstance().getTopologicallySortedProcNames();
    for (auto it = sortedProcNames.rbegin(); it != sortedProcNames.rend(); it++) {
        Procedure *proc = pkb->getProcByName(*it);
        extractProcedure(proc);
    }
}

void NextBipExtractor::extractProcedure(Procedure *procedure) {
    ExtractionContext::getInstance().setCurrentProcedure(procedure);
    // Create terminal stmt (equivalent of dummy node in CFG)
    auto *terminalStmt = new Statement(getNextTerminalIndex(), StatementType::TERMINAL);
    terminalStmtsMap[procedure->getId()] = terminalStmt;

    // Extract statements recursively
    Statement *firstExecutableStmt = pkb->getProcByName(procedure->getId())->getStmtLst().front();
    extractStatement(firstExecutableStmt);

    ExtractionContext::getInstance().unsetCurrentProcedure(procedure);
}

void NextBipExtractor::extractStatement(Statement *statement) {
    // If previously extracted, do nothing
    ProgLineIndex index = statement->getId();
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
    StmtIndex callStmtIndex = callStatement->getId();

    auto calledProcFirstExecutableStmt = getFirstExecutableStmt(calledProcName);
    auto calledProcTerminalStmt = terminalStmtsMap[calledProcName];
    auto nextStatement = getStatementAfterCallStatement(callStmtIndex);

    if (!nextStatement.has_value()) {
        extractCallTerminalNextBip(callStatement, calledProcFirstExecutableStmt,
                                   calledProcTerminalStmt);
    } else {
        extractCallStatementNextBip(callStatement, calledProcFirstExecutableStmt,
                                    calledProcTerminalStmt, nextStatement.value());
        extractStatement(nextStatement.value());
    }
}

void NextBipExtractor::extractNonCallStatement(Statement *statement) {
    if (isLastExecutableStmt(statement)) {
        extractNonCallTerminalNextBip(statement);
    }
    set<ProgLineIndex> nextStatements = pkb->getNextLines(statement->getId());
    for (ProgLineIndex nextStatementIndex : nextStatements) {
        Statement *nextStatement;
        if (nextStatementIndex <= STARTING_TERMINAL_INDEX) {
            nextStatement = terminalStmtsMap[getCurrentProcName()];
        } else {
            nextStatement = pkb->getStmtByIndex(nextStatementIndex);
        }
        pkb->insertNextBip(statement, nextStatement);
        extractStatement(nextStatement);
    }
}

void NextBipExtractor::extractCallStatementNextBip(Statement *branchInFrom, Statement *branchInTo,
                                                   Statement *branchBackFrom,
                                                   Statement *branchBackTo) {
    pkb->insertBranchIn(branchInFrom, branchInTo);
    pkb->insertNextBip(branchInFrom, branchInTo);
    pkb->insertNextBip(branchBackFrom, branchBackTo);
    pkb->insertBranchBack(branchBackFrom, branchBackTo);
}

Statement *NextBipExtractor::getFirstExecutableStmt(const ProcName &procName) {
    StmtIndex calledProcFirstStmtIndex =
        ExtractionContext::getInstance().getFirstExecutableStatement(procName);
    return pkb->getStmtByIndex(calledProcFirstStmtIndex);
}

optional<Statement *> NextBipExtractor::getStatementAfterCallStatement(StmtIndex callStmtIndex) {
    set<ProgLineIndex> nextStmtIndices = pkb->getNextLines(callStmtIndex);
    if (nextStmtIndices.empty()) {
        return nullopt;
    }
    if (nextStmtIndices.size() > 1) {
        throw runtime_error("Encountered a call statement with more than one next "
                            "statement (syntactically impossible).");
    }
    return pkb->getStmtByIndex(*nextStmtIndices.begin());
}

ProcName NextBipExtractor::getCurrentProcName() {
    auto currentProc = ExtractionContext::getInstance().getCurrentProcedure();
    if (!currentProc.has_value()) {
        throw runtime_error("Current procedure not set.");
    }
    return currentProc.value()->getId();
}

void NextBipExtractor::extractNonCallTerminalNextBip(Statement *statement) {
    ProcName currentProcName = getCurrentProcName();
    if (!terminalStmtsMap.count(currentProcName)) {
        throw runtime_error("Current procedure terminal stmt not set.");
    }

    pkb->insertNext(statement, terminalStmtsMap[currentProcName]);
    pkb->insertNextBip(statement, terminalStmtsMap[currentProcName]);
    visited.insert(terminalStmtsMap[currentProcName]->getId());
}

void NextBipExtractor::extractCallTerminalNextBip(Statement *branchInFrom, Statement *branchInTo,
                                                  Statement *branchBackFrom) {
    ProcName currentProcName = getCurrentProcName();
    if (!terminalStmtsMap.count(currentProcName)) {
        throw runtime_error("Current procedure terminal stmt not set.");
    }
    Statement *branchBackTo = terminalStmtsMap[currentProcName];
    pkb->insertNext(branchInFrom, branchBackTo);
    extractCallStatementNextBip(branchInFrom, branchInTo, branchBackFrom, branchBackTo);
    visited.insert(terminalStmtsMap[currentProcName]->getId());
}

bool NextBipExtractor::isLastExecutableStmt(Statement *statement) {
    set<ProgLineIndex> nextStatements = pkb->getNextLines(statement->getId());
    return (nextStatements.empty() ||
            (statement->getStatementType() == StatementType::WHILE && nextStatements.size() < 2));
}
