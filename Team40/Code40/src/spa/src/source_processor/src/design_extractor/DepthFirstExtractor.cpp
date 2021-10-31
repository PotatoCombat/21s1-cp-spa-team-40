#include "source_processor/design_extractor/DepthFirstExtractor.h"

DepthFirstExtractor::DepthFirstExtractor(PKB *pkb) : pkb(pkb) {}

void DepthFirstExtractor::extract(Program *program) {
    ExtractionContext::getInstance().reset();
    for (Procedure *procedure : program->getProcLst()) {
        extractProcedure(procedure);
    }
}

void DepthFirstExtractor::extractProcedure(Procedure *procedure) {
    ExtractionContext::getInstance().setCurrentProcedure(procedure);
    if (pkb->getProcByName(procedure->getName()) != nullptr) {
        throw runtime_error("Encountered a Procedure with a duplicate name.");
    };
    pkb->insertProc(procedure);
    for (Statement *statement : procedure->getStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().unsetCurrentProcedure(procedure);
    // Next is procedure-scoped, clear all previous statements
    ExtractionContext::getInstance().clearPreviousStatements();
}

void DepthFirstExtractor::extractStatement(Statement *statement) {
    pkb->insertStmt(statement);

    // Handle Parents(s1, s2)
    vector<Statement *> parentStatement =
        ExtractionContext::getInstance().getParentStatements();
    if (!parentStatement.empty()) {
        pkb->insertParent(parentStatement.back(), statement);
    }

    // Handle Next(s1, s2)
    vector<Statement *> previousStatements =
        ExtractionContext::getInstance().getPreviousStatements();
    for (Statement *previousStatement : previousStatements) {
        pkb->insertNext(previousStatement, statement);
    }
    ExtractionContext::getInstance().clearPreviousStatements();

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

    // Handle LHS
    ExtractionContext::getInstance().setModifyingStatement(assignStatement);
    extractVariable(assignStatement->getVariable());
    ExtractionContext::getInstance().unsetModifyingStatement(assignStatement);

    // Handle RHS
    ExtractionContext::getInstance().setUsingStatement(assignStatement);
    for (Variable *variable : assignStatement->getExpressionVars()) {
        extractVariable(variable);
    }
    for (ConstantValue *constantValue :
         assignStatement->getExpressionConsts()) {
        extractConstantValue(constantValue);
    }
    ExtractionContext::getInstance().unsetUsingStatement(assignStatement);
    ExtractionContext::getInstance().setPreviousStatement(assignStatement);
}

void DepthFirstExtractor::extractCallStatement(Statement *callStatement) {

    ProcName calledProcName = callStatement->getProcName();

    optional<Procedure *> currentProcedure =
        ExtractionContext::getInstance().getCurrentProcedure();
    if (!currentProcedure.has_value()) {
        throw runtime_error("Current procedure not set.");
    }
    ExtractionContext::getInstance().registerProcDependency(
        currentProcedure.value()->getName(), calledProcName);
    ExtractionContext::getInstance().setPreviousStatement(callStatement);
}

void DepthFirstExtractor::extractIfStatement(Statement *ifStatement) {
    pkb->insertIfPattern(ifStatement);

    // 1. Handle condition
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

    // 2. Handle THEN statements
    ExtractionContext::getInstance().clearPreviousStatements();
    ExtractionContext::getInstance().setPreviousStatement(ifStatement);
    ExtractionContext::getInstance().setParentStatement(ifStatement);
    for (Statement *statement : ifStatement->getThenStmtLst()) {
        extractStatement(statement);
    }

    // 3. Handle ELSE statements
    ExtractionContext::getInstance().clearPreviousStatements();
    ExtractionContext::getInstance().setPreviousStatement(ifStatement);
    for (Statement *statement : ifStatement->getElseStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().clearPreviousStatements();

    // Set all last non-while, non-if (a.k.a. leaf) statements in the THEN/ELSE
    // statement lists as previous statements
    vector<Statement *> lastExecutedStatements;
    extractLastExecutedStatement(ifStatement->getThenStmtLst(),
                                 lastExecutedStatements);
    extractLastExecutedStatement(ifStatement->getElseStmtLst(),
                                 lastExecutedStatements);
    for (Statement *lastExecutedStatement : lastExecutedStatements) {
        ExtractionContext::getInstance().setPreviousStatement(
            lastExecutedStatement);
    }

    ExtractionContext::getInstance().unsetParentStatement(ifStatement);
}

/**
 * Recursively extracts the last executed statement in a statement list
 */
void DepthFirstExtractor::extractLastExecutedStatement(
    vector<Statement *> statementList, vector<Statement *> &result) {
    Statement *lastStatement = statementList.back();
    if (lastStatement->getStatementType() == StatementType::IF) {
        extractLastExecutedStatement(lastStatement->getThenStmtLst(), result);
        extractLastExecutedStatement(lastStatement->getElseStmtLst(), result);
    } else {
        result.push_back(lastStatement);
    }
}

void DepthFirstExtractor::extractReadStatement(Statement *readStatement) {
    ExtractionContext::getInstance().setModifyingStatement(readStatement);
    extractVariable(readStatement->getVariable());
    ExtractionContext::getInstance().unsetModifyingStatement(readStatement);

    ExtractionContext::getInstance().setPreviousStatement(readStatement);
}

void DepthFirstExtractor::extractPrintStatement(Statement *printStatement) {
    ExtractionContext::getInstance().setUsingStatement(printStatement);
    extractVariable(printStatement->getVariable());
    ExtractionContext::getInstance().unsetUsingStatement(printStatement);

    ExtractionContext::getInstance().setPreviousStatement(printStatement);
}

void DepthFirstExtractor::extractWhileStatement(Statement *whileStatement) {
    pkb->insertWhilePattern(whileStatement);

    // 1. Handle condition
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

    // 2. Handle THEN statements
    ExtractionContext::getInstance().setParentStatement(whileStatement);
    ExtractionContext::getInstance().setPreviousStatement(whileStatement);
    for (Statement *statement : whileStatement->getThenStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().unsetParentStatement(whileStatement);

    // Handle Next(w, s) where stmt s are all the last non-while, non-if (a.k.a.
    // leaf) statements in the THEN statement list belonging to while w
    vector<Statement *> lastExecutedStatements;
    extractLastExecutedStatement(whileStatement->getThenStmtLst(),
                                 lastExecutedStatements);
    for (Statement *lastExecutedStatement : lastExecutedStatements) {
        pkb->insertNext(lastExecutedStatement, whileStatement);
    }
    ExtractionContext::getInstance().clearPreviousStatements();
    ExtractionContext::getInstance().setPreviousStatement(whileStatement);
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
