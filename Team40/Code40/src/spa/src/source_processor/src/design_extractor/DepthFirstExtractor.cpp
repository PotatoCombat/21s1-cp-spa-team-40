#include "source_processor/design_extractor/DepthFirstExtractor.h"

DepthFirstExtractor::DepthFirstExtractor(PKB *pkb) : pkb(pkb) {}

void DepthFirstExtractor::extract(Program *program) {
    ExtractionContext::getInstance().reset();
    for (Procedure *procedure : program->getProcLst()) {
        extractProcedure(procedure);
    }
}

void DepthFirstExtractor::extractProcedure(Procedure *procedure) {
    ProcName procName = procedure->getName();
    if (pkb->getProcByName(procName) != nullptr) {
        throw runtime_error("Encountered a Procedure with a duplicate name.");
    }
    pkb->insertProc(procedure);

    // Set first-executable statement in ExtractionContext
    StmtIndex firstStmtIndex = procedure->getStmtLst().front()->getIndex();
    ExtractionContext::getInstance().setFirstExecutableStatement(
        procName, firstStmtIndex);

    // Extract each statement in procedure
    ExtractionContext::getInstance().setCurrentProcedure(procedure);
    for (Statement *statement : procedure->getStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().unsetCurrentProcedure(procedure);
    // Next is procedure-scoped, clear all previous statements
    ExtractionContext::getInstance().clearPreviousStatements();
}

void DepthFirstExtractor::extractStatement(Statement *statement) {
    pkb->insertStmt(statement);

    extractParentRelationship(statement);
    extractNextRelationship(statement);

    switch (statement->getStatementType()) {
    case StatementType::ASSIGN:
        extractAssignStatement(statement);
        break;
    case StatementType::CALL:
        extractCallStatement(statement);
        break;
    case StatementType::IF:
        extractIfStatement(statement);
        break;
    case StatementType::PRINT:
        extractPrintStatement(statement);
        break;
    case StatementType::READ:
        extractReadStatement(statement);
        break;
    case StatementType::WHILE:
        extractWhileStatement(statement);
        break;
    default:
        throw runtime_error("Invalid StatementType!");
    }
}

void DepthFirstExtractor::extractAssignStatement(Statement *assignStatement) {
    pkb->insertPatternAssign(assignStatement);

    extractAssignStatementLHS(assignStatement);
    extractAssignStatementRHS(assignStatement);

    ExtractionContext::getInstance().unsetUsingStatement(assignStatement);
    ExtractionContext::getInstance().setPreviousStatement(assignStatement);
    updateLastExecutableStatementsForCurrentProc({assignStatement->getIndex()});
}

void DepthFirstExtractor::extractAssignStatementLHS(
    Statement *assignStatement) {
    ExtractionContext::getInstance().setModifyingStatement(assignStatement);
    extractVariable(assignStatement->getVariable());
    ExtractionContext::getInstance().unsetModifyingStatement(assignStatement);
}

void DepthFirstExtractor::extractAssignStatementRHS(
    Statement *assignStatement) {
    ExtractionContext::getInstance().setUsingStatement(assignStatement);
    for (Variable *variable : assignStatement->getExpressionVars()) {
        extractVariable(variable);
    }
    for (ConstantValue *constantValue :
         assignStatement->getExpressionConsts()) {
        extractConstantValue(constantValue);
    }
}

void DepthFirstExtractor::extractCallStatement(Statement *callStatement) {

    extractProcDependency(callStatement);
    ExtractionContext::getInstance().setPreviousStatement(callStatement);
    updateLastExecutableStatementsForCurrentProc({callStatement->getIndex()});
}

void DepthFirstExtractor::extractProcDependency(Statement *callStatement) {
    ProcName calledProcName = callStatement->getProcName();
    optional<Procedure *> currentProcedure =
        ExtractionContext::getInstance().getCurrentProcedure();
    if (!currentProcedure.has_value()) {
        throw runtime_error("Current procedure not set.");
    }
    ExtractionContext::getInstance().registerProcDependency(
        currentProcedure.value()->getName(), calledProcName);
}

void DepthFirstExtractor::extractIfStatement(Statement *ifStatement) {

    extractIfCondition(ifStatement);
    extractIfThenStatements(ifStatement);
    extractIfElseStatements(ifStatement);
    extractIfStatementCleanup(ifStatement);
    ExtractionContext::getInstance().unsetParentStatement(ifStatement);
}

void DepthFirstExtractor::extractIfCondition(Statement *ifStatement) {
    pkb->insertIfPattern(ifStatement);

    auto variables = ifStatement->getExpressionVars();
    auto constantValues = ifStatement->getExpressionConsts();
    if (variables.empty() && constantValues.empty()) {
        throw runtime_error(to_string(ifStatement->getIndex()) +
                            ": IfStatement is missing cond_expr.");
    }
    ExtractionContext::getInstance().setUsingStatement(ifStatement);
    for (Variable *variable : variables) {
        extractVariable(variable);
    }
    for (ConstantValue *constantValue : constantValues) {
        extractConstantValue(constantValue);
    }
    ExtractionContext::getInstance().unsetUsingStatement(ifStatement);
}

void DepthFirstExtractor::extractIfThenStatements(Statement *ifStatement) {
    ExtractionContext::getInstance().clearPreviousStatements();
    ExtractionContext::getInstance().setPreviousStatement(ifStatement);
    ExtractionContext::getInstance().setParentStatement(ifStatement);
    for (Statement *statement : ifStatement->getThenStmtLst()) {
        extractStatement(statement);
    }
}

void DepthFirstExtractor::extractIfElseStatements(Statement *ifStatement) {
    ExtractionContext::getInstance().clearPreviousStatements();
    ExtractionContext::getInstance().setPreviousStatement(ifStatement);
    for (Statement *statement : ifStatement->getElseStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().clearPreviousStatements();
}

void DepthFirstExtractor::extractIfStatementCleanup(Statement *ifStatement) {
    // Set all last non-while, non-if (a.k.a. leaf) statements in the THEN/ELSE
    // statement lists as previous statements
    vector<Statement *> lastExecutableStatements;
    extractLastExecutableStatement(ifStatement->getThenStmtLst(),
                                   lastExecutableStatements);
    extractLastExecutableStatement(ifStatement->getElseStmtLst(),
                                   lastExecutableStatements);
    set<StmtIndex> lastExecutableStmtIndices;
    for (Statement *lastExecutableStatement : lastExecutableStatements) {
        ExtractionContext::getInstance().setPreviousStatement(
            lastExecutableStatement);
        lastExecutableStmtIndices.insert(lastExecutableStatement->getIndex());
    }
    updateLastExecutableStatementsForCurrentProc(lastExecutableStmtIndices);
}

/**
 * Recursively extracts the last executable statement in a statement list
 */
void DepthFirstExtractor::extractLastExecutableStatement(
    vector<Statement *> statementList, vector<Statement *> &result) {
    Statement *lastStatement = statementList.back();
    if (lastStatement->getStatementType() == StatementType::IF) {
        extractLastExecutableStatement(lastStatement->getThenStmtLst(), result);
        extractLastExecutableStatement(lastStatement->getElseStmtLst(), result);
    } else {
        result.push_back(lastStatement);
    }
}

void DepthFirstExtractor::extractReadStatement(Statement *readStatement) {
    ExtractionContext::getInstance().setModifyingStatement(readStatement);
    extractVariable(readStatement->getVariable());
    ExtractionContext::getInstance().unsetModifyingStatement(readStatement);
    ExtractionContext::getInstance().setPreviousStatement(readStatement);
    updateLastExecutableStatementsForCurrentProc({readStatement->getIndex()});
}

void DepthFirstExtractor::extractPrintStatement(Statement *printStatement) {
    ExtractionContext::getInstance().setUsingStatement(printStatement);
    extractVariable(printStatement->getVariable());
    ExtractionContext::getInstance().unsetUsingStatement(printStatement);

    ExtractionContext::getInstance().setPreviousStatement(printStatement);
    updateLastExecutableStatementsForCurrentProc({printStatement->getIndex()});
}

void DepthFirstExtractor::extractWhileStatement(Statement *whileStatement) {

    extractWhileCondition(whileStatement);
    extractWhileThenStatements(whileStatement);
    extractWhileStatementCleanup(whileStatement);
}

void DepthFirstExtractor::extractWhileCondition(Statement *whileStatement) {
    pkb->insertWhilePattern(whileStatement);

    auto variables = whileStatement->getExpressionVars();
    auto constantValues = whileStatement->getExpressionConsts();
    if (variables.empty() && constantValues.empty()) {
        throw runtime_error(to_string(whileStatement->getIndex()) +
                            ": WhileStatement is missing cond_expr.");
    }

    ExtractionContext::getInstance().setUsingStatement(whileStatement);
    for (Variable *variable : variables) {
        extractVariable(variable);
    }
    for (ConstantValue *constantValue : constantValues) {
        extractConstantValue(constantValue);
    }
    ExtractionContext::getInstance().unsetUsingStatement(whileStatement);
}

void DepthFirstExtractor::extractWhileThenStatements(
    Statement *whileStatement) {
    ExtractionContext::getInstance().setParentStatement(whileStatement);
    ExtractionContext::getInstance().setPreviousStatement(whileStatement);
    for (Statement *statement : whileStatement->getThenStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().unsetParentStatement(whileStatement);
}

void DepthFirstExtractor::extractWhileStatementCleanup(
    Statement *whileStatement) {
    // Handle Next(w, s) where stmt s are all the last non-while, non-if (a.k.a.
    // leaf) statements in the THEN statement list belonging to while w
    vector<Statement *> lastExecutableStatements;
    extractLastExecutableStatement(whileStatement->getThenStmtLst(),
                                   lastExecutableStatements);
    for (Statement *lastExecutableStatement : lastExecutableStatements) {
        pkb->insertNext(lastExecutableStatement, whileStatement);
    }

    // Set whileStatement as the only previous statement
    ExtractionContext::getInstance().clearPreviousStatements();
    ExtractionContext::getInstance().setPreviousStatement(whileStatement);
    updateLastExecutableStatementsForCurrentProc({whileStatement->getIndex()});
}

void DepthFirstExtractor::extractVariable(Variable *variable) {
    pkb->insertVar(variable);
    extractUsesRelationship(variable);
    extractModifiesRelationship(variable);
}

void DepthFirstExtractor::extractUsesRelationship(Variable *variable) {
    optional<Statement *> usingStatement =
        ExtractionContext::getInstance().getUsingStatement();
    if (!usingStatement.has_value()) {
        return;
    }

    // 1. Handle using statement
    pkb->insertStmtUsingVar(usingStatement.value(), variable);

    // 2. Handle all parent statements
    vector<Statement *> parentStatements =
        ExtractionContext::getInstance().getParentStatements();
    if (!parentStatements.empty()) {
        for (Statement *parentStatement : parentStatements) {
            pkb->insertStmtUsingVar(parentStatement, variable);
        }
    }

    // 3. Handle current procedure
    optional<Procedure *> currentProcedure =
        ExtractionContext::getInstance().getCurrentProcedure();
    if (!currentProcedure.has_value()) {
        throw runtime_error("Current procedure not set.");
    }
    pkb->insertProcUsingVar(currentProcedure.value(), variable);

    /// NOTE: Transitivity with other Procedures via CallStatements is handled
    /// in the BreadthFirstExtractor::extractCallStatement since we have yet
    /// to form the procedure dependency tree at this point
}

void DepthFirstExtractor::extractModifiesRelationship(Variable *variable) {
    optional<Statement *> modifyingStatement =
        ExtractionContext::getInstance().getModifyingStatement();
    if (!modifyingStatement.has_value()) {
        return;
    }

    // 1. Handle modifying statement
    pkb->insertStmtModifyingVar(modifyingStatement.value(), variable);

    // 2. Handle all parent statements
    vector<Statement *> parentStatements =
        ExtractionContext::getInstance().getParentStatements();
    if (!parentStatements.empty()) {
        for (Statement *parentStatement : parentStatements) {
            pkb->insertStmtModifyingVar(parentStatement, variable);
        }
    }

    // 3. Handle current procedure
    optional<Procedure *> currentProcedure =
        ExtractionContext::getInstance().getCurrentProcedure();
    if (!currentProcedure.has_value()) {
        throw runtime_error("Current procedure not set.");
    }
    pkb->insertProcModifyingVar(currentProcedure.value(), variable);

    /// NOTE: Transitivity with other Procedures via CallStatements is handled
    /// in the BreadthFirstExtractor::extractCallStatement since we have yet
    /// to form the procedure dependency tree at this point
}

void DepthFirstExtractor::extractConstantValue(ConstantValue *constantValue) {
    pkb->insertConst(constantValue);
}

void DepthFirstExtractor::updateLastExecutableStatementsForCurrentProc(
    set<ProgLineIndex> lastExecutableStmtIndices) {
    auto curProc = ExtractionContext::getInstance().getCurrentProcedure();
    if (!curProc.has_value()) {
        throw runtime_error("Current procedure not set.");
    }
    ProcName curProcName = curProc.value()->getName();
    ExtractionContext::getInstance().setLastExecutableStatements(
        curProcName, std::move(lastExecutableStmtIndices));
}

void DepthFirstExtractor::extractParentRelationship(Statement *statement) {
    vector<Statement *> parentStatement =
        ExtractionContext::getInstance().getParentStatements();
    if (!parentStatement.empty()) {
        pkb->insertParent(parentStatement.back(), statement);
    }
}

void DepthFirstExtractor::extractNextRelationship(Statement *statement) {
    vector<Statement *> previousStatements =
        ExtractionContext::getInstance().getPreviousStatements();
    for (Statement *previousStatement : previousStatements) {
        pkb->insertNext(previousStatement, statement);
    }
    ExtractionContext::getInstance().clearPreviousStatements();
}