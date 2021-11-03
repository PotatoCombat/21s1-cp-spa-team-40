#include "source_processor/design_extractor/BreadthFirstExtractor.h"

BreadthFirstExtractor::BreadthFirstExtractor(PKB *pkb) : pkb(pkb){};

void BreadthFirstExtractor::extract(Program *program) {
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

void BreadthFirstExtractor::extractProcedure(Procedure *procedure) {
    ExtractionContext::getInstance().setCurrentProcedure(procedure);
    statementLists.push_back(procedure->getStmtLst());
    while (!statementLists.empty()) {
        ExtractionContext::getInstance().clearPrecedingStatements();
        vector<Statement *> statementList = statementLists.back();
        statementLists.pop_back();
        for (Statement *statement : statementList) {
            extractStatement(statement);
        }
    }
    ExtractionContext::getInstance().unsetCurrentProcedure(procedure);
}

void BreadthFirstExtractor::extractStatement(Statement *statement) {
    vector<Statement *> precedingStatements =
        ExtractionContext::getInstance().getPrecedingStatements();
    if (!precedingStatements.empty()) {
        pkb->insertFollows(precedingStatements.back(), statement);
    }
    switch (statement->getStatementType()) {
    case StatementType::WHILE:
        extractWhileStatement(statement);
        break;
    case StatementType::IF:
        extractIfStatement(statement);
        break;
    case StatementType::CALL:
        extractCallStatement(statement);
        break;
    case StatementType::ASSIGN:
    case StatementType::PRINT:
    case StatementType::READ:
        break;
    default:
        throw runtime_error("Invalid StatementType!");
    }
    ExtractionContext::getInstance().setPrecedingStatement(statement);
}

void BreadthFirstExtractor::extractIfStatement(Statement *ifStatement) {
    statementLists.push_back(ifStatement->getThenStmtLst());
    statementLists.push_back(ifStatement->getElseStmtLst());
}

void BreadthFirstExtractor::extractWhileStatement(Statement *whileStatement) {
    statementLists.push_back(whileStatement->getThenStmtLst());
}

void BreadthFirstExtractor::extractCallStatement(Statement *callStatement) {
    optional<Procedure *> currentProcedure =
        ExtractionContext::getInstance().getCurrentProcedure();
    if (!currentProcedure.has_value()) {
        throw runtime_error("Current procedure not set");
    }

    ProcName calleeName = callStatement->getProcName();
    if (pkb->getProcByName(calleeName) == nullptr) {
        throw runtime_error("Trying to call a non-existent procedure at line " +
                            to_string(callStatement->getIndex()));
    }

    // Extract Calls(proc, proc) relationship
    pkb->insertCalls(currentProcedure.value(), calleeName);

    extractTransitiveUsesRelationship(callStatement, currentProcedure.value(),
                                      calleeName);
    extractTransitiveModifiesRelationship(callStatement,
                                          currentProcedure.value(), calleeName);
    expandLastExecutableCallStatements(callStatement, currentProcedure.value(),
                                       calleeName);
}

void BreadthFirstExtractor::extractTransitiveModifiesRelationship(
    Statement *callStatement, Procedure *currentProcedure,
    ProcName calleeName) {

    set<VarName> modifiedVarNames =
        pkb->getVarsModifiedByProc(std::move(calleeName));
    for (const VarName &modifiedVarName : modifiedVarNames) {
        Variable *modifiedVar = pkb->getVarByName(modifiedVarName);
        // If Modifies(calleeProc, var) then Modifies(callerProc, var)
        pkb->insertProcModifyingVar(currentProcedure, modifiedVar);
        pkb->insertStmtModifyingVar(callStatement, modifiedVar);
        // If Modifies(stmt, var) then Modifies(parentStarStmt, var)
        for (StmtIndex parentStarStatement :
             pkb->getParentStarStmts(callStatement->getIndex())) {
            pkb->insertStmtModifyingVar(
                pkb->getStmtByIndex(parentStarStatement), modifiedVar);
        }
    }
}

void BreadthFirstExtractor::extractTransitiveUsesRelationship(
    Statement *callStatement, Procedure *currentProcedure,
    ProcName calleeName) {

    set<VarName> usedVarNames = pkb->getVarsUsedByProc(std::move(calleeName));
    for (const VarName &usedVarName : usedVarNames) {
        Variable *usedVar = pkb->getVarByName(usedVarName);
        // If Uses(calleeProc, var) then Uses(callerProc, var)
        pkb->insertProcUsingVar(currentProcedure, usedVar);
        pkb->insertStmtUsingVar(callStatement, usedVar);
        // If Uses(stmt, var) then Uses(parentStarStmt, var)
        for (StmtIndex parentStarStatement :
             pkb->getParentStarStmts(callStatement->getIndex())) {
            pkb->insertStmtUsingVar(pkb->getStmtByIndex(parentStarStatement),
                                    usedVar);
        }
    }
}

/**
 * If the given call statement is (one of) the last-executable statement found
 * by the DepthFirstExtractor for the current procedure, replace it with those
 * of the called procedure.
 */
void BreadthFirstExtractor::expandLastExecutableCallStatements(
    Statement *callStatement, Procedure *currentProcedure,
    const ProcName &calleeName) {

    ProcName curProcName = currentProcedure->getName();
    StmtIndex curStmtIndex = callStatement->getIndex();

    if (ExtractionContext::getInstance()
            .getLastExecutableStatements(curProcName)
            .count(curStmtIndex)) {
        auto curProcLastExecutableStmts =
            ExtractionContext::getInstance().getLastExecutableStatements(
                curProcName);
        auto calleeLastExecutableStmts =
            ExtractionContext::getInstance().getLastExecutableStatements(
                calleeName);

        curProcLastExecutableStmts.erase(curStmtIndex);
        curProcLastExecutableStmts.insert(calleeLastExecutableStmts.begin(),
                                          calleeLastExecutableStmts.end());

        ExtractionContext::getInstance().setLastExecutableStatements(
            curProcName, curProcLastExecutableStmts);
    }
}