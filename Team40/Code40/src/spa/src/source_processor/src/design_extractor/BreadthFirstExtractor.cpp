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
        if (procMap.count(procName)) {
            extractProcedure(procMap[procName]);
        }
    }
}

void BreadthFirstExtractor::extractProcedure(Procedure *procedure) {
    ExtractionContext::getInstance().setCurrentProcedure(procedure);
    statementLists.push_back(procedure->getStmtLst());
    while (!statementLists.empty()) {
        vector<Statement *> statementList = statementLists.back();
        statementLists.pop_back();
        for (Statement *statement : statementList) {
            extractStatement(statement);
        }
    }
    ExtractionContext::getInstance().unsetCurrentProcedure(procedure);
}

void BreadthFirstExtractor::extractStatement(Statement *statement) {
    auto previousStatements =
        ExtractionContext::getInstance().getFollowsContext().getCopy();
    for (Statement *prev : previousStatements) {
        pkb->insertFollows(prev, statement);
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
    default:
        throw runtime_error("Invalid StatementType!");
    }
    ExtractionContext::getInstance().getFollowsContext().push(statement);
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
        ExtractionContext::getInstance().getCurrentProcedure().value();
    if (!currentProcedure.has_value()) {
        throw runtime_error("Current procedure not set");
    }
    ProcName calleeName = callStatement->getProcName();

    // Handle transitive Modifies(proc, var) relationship set<VarName>
    set<VarName> modifiedVarNames = pkb->getVarsModifiedByProc(calleeName);
    for (VarName modifiedVarName : modifiedVarNames) {
        Variable *modifiedVar = pkb->getVarByName(modifiedVarName);
        pkb->insertProcModifyingVar(currentProcedure.value(), modifiedVar);
    }

    // Handle transitive Uses(proc, var) relationship
    set<VarName> usedVarNames = pkb->getVarsUsedByProc(calleeName);
    for (VarName usedVarName : usedVarNames) {
        Variable *usedVar = pkb->getVarByName(usedVarName);
        pkb->insertProcUsingVar(currentProcedure.value(), usedVar);
    }
}
