#include "source_processor/design_extractor/BreadthFirstExtractor.h"

BreadthFirstExtractor::BreadthFirstExtractor(PKB *pkb) : pkb(pkb){};

void BreadthFirstExtractor::extract(Program *program) {
    ExtractionContext::getInstance().resetTransientContexts();

    unordered_map<ProcName, Procedure *> procMap;
    for (Procedure *procedure : program->getProcLst()) {
        procMap[procedure->getName()] = procedure;
    }
    vector<ProcName> sortedProcNames =
        ExtractionContext::getInstance().getTopologicallySortedProcNames();
    for (auto it = sortedProcNames.rbegin(); it != sortedProcNames.rend();
         it++) {
        string procName = *it;
        extractProcedure(procMap[procName]);
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
    ExtractionContext::getInstance().setParentStatement(ifStatement);
    ExtractionContext::getInstance().setParentStatement(ifStatement);
    statementLists.push_back(ifStatement->getThenStmtLst());
    statementLists.push_back(ifStatement->getElseStmtLst());
}

void BreadthFirstExtractor::extractWhileStatement(Statement *whileStatement) {
    ExtractionContext::getInstance().setParentStatement(whileStatement);
    statementLists.push_back(whileStatement->getThenStmtLst());
}

void BreadthFirstExtractor::extractCallStatement(Statement *callStatement) {
    optional<Procedure *> currentProcedure =
        ExtractionContext::getInstance().getCurrentProcedure().value();
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

    Statement *parentStatement = nullptr;
    if (!ExtractionContext::getInstance().getParentStatements().empty()) {
        parentStatement =
            ExtractionContext::getInstance().getParentStatements().back();
    }

    // Handle transitive Modifies
    set<VarName> modifiedVarNames = pkb->getVarsModifiedByProc(calleeName);
    for (VarName modifiedVarName : modifiedVarNames) {
        Variable *modifiedVar = pkb->getVarByName(modifiedVarName);
        // If Modifies(calleeProc, var) then Modifies(callerProc, var)
        pkb->insertProcModifyingVar(currentProcedure.value(), modifiedVar);
        if (parentStatement != nullptr) {
            // If Modifies(stmt, var) then Modifies(parentStmt, var)
            pkb->insertStmtModifyingVar(parentStatement, modifiedVar);
        }
    }

    // Handle transitive Uses(proc, var)
    set<VarName> usedVarNames = pkb->getVarsUsedByProc(calleeName);
    for (VarName usedVarName : usedVarNames) {
        Variable *usedVar = pkb->getVarByName(usedVarName);
        // If Uses(calleeProc, var) then Uses(callerProc, var)
        pkb->insertProcUsingVar(currentProcedure.value(), usedVar);
        if (parentStatement != nullptr) {
            // If Uses(stmt, var) then Uses(parentStmt, var)
            pkb->insertStmtUsingVar(parentStatement, usedVar);
        }
    }
    if (parentStatement != nullptr) {
        ExtractionContext::getInstance().unsetParentStatement(parentStatement);
    }
}
