#include "source_processor/DesignExtractor.h"

using namespace std;

DesignExtractor::DesignExtractor(PKB pkb) : pkb(pkb) {}

void DesignExtractor::handleProgram(Program program) {
    for (Procedure proc : program.getProcLst()) {
        handleProcedure(&proc);
    }
}

ProcIndex DesignExtractor::handleProcedure(Procedure *procedure) {
    ProcIndex procIndex = pkb.insertProc(procedure);
    ExtractionContext::getInstance().setCurrentProc(procIndex);
    for (Statement statement : procedure->getStmtLst()) {
        handleStatement(&statement);
    }
    return procIndex;
}

StmtIndex DesignExtractor::handleStatement(Statement *statement) {
    StmtIndex stmtIndex;
    switch (statement->getStatementType()) {
    case StatementType::ASSIGN:
        stmtIndex = handleAssignStatement(statement);
        break;
    case StatementType::CALL:
        stmtIndex = handleCallStatement(statement);
        break;
    case StatementType::IF:
        stmtIndex = handleIfStatement(statement);
        break;
    case StatementType::READ:
    case StatementType::PRINT:
    case StatementType::WHILE:
        // TODO: note to unset prev, and set parent
    default:
        throw runtime_error("Invalid StatementType!");
    }
    handleContextualRelationships(stmtIndex);
    return stmtIndex;
}

void DesignExtractor::handleContextualRelationships(StmtIndex stmtIndex) {
    Statement *statement = pkb.getStmtByIndex(stmtIndex);
    optional<StmtIndex> prevStmtIndex =
        ExtractionContext::getInstance().getPrevStatement();
    if (prevStmtIndex.has_value()) {
        Statement *prevStmt = pkb.getStmtByIndex(prevStmtIndex.value());
        pkb.insertFollows(prevStmt, statement);
    }

    optional<StmtIndex> parentStmtIndex =
        ExtractionContext::getInstance().getParentStatement();

    if (parentStmtIndex.has_value()) {
        Statement *parentStmt = pkb.getStmtByIndex(parentStmtIndex.value());
        pkb.insertParent(parentStmt, statement);
    }

    ExtractionContext::getInstance().setPrevStatement(stmtIndex);
}

StmtIndex DesignExtractor::handleAssignStatement(Statement *assignStatement) {
    StmtIndex stmtIndex = pkb.insertStmt(assignStatement);
    Variable *var = assignStatement->getVariable();
    VarIndex varIdx = pkb.insertVar(var);
    pkb.insertStmtModifyingVar(assignStatement, var);

    ExtractionContext::getInstance().setModifyingStatement(stmtIndex);
    handleVariable(assignStatement->getVariable());
    ExtractionContext::getInstance().unsetModifyingStatement();

    ExtractionContext::getInstance().setUsingStatement(stmtIndex);
    handleExpression(assignStatement->getExpression());
    ExtractionContext::getInstance().unsetUsingStatement();

    return stmtIndex;
}

StmtIndex DesignExtractor::handleCallStatement(Statement *callStatement) {
    StmtIndex stmtIndex = pkb.insertStmt(callStatement);
    return stmtIndex;
}

StmtIndex DesignExtractor::handleIfStatement(Statement *ifStatement) {
    // 0. Insert statement into PKB
    StmtIndex stmtIndex = pkb.insertStmt(ifStatement);

    // 1. Handle condition
    ExtractionContext::getInstance().setUsingStatement(stmtIndex);
    handleCondition(ifStatement->getCondition());
    ExtractionContext::getInstance().unsetUsingStatement();

    ExtractionContext::getInstance().setParentStatement(stmtIndex);
    // 2. Handle THEN statements
    for (Statement *statement : ifStatement->getThenStmtLst()) {
        handleStatement(statement);
    }

    // 3. Handle ELSE statements
    for (Statement *statement : ifStatement->getElseStmtLst()) {
        handleStatement(statement);
    }
    ExtractionContext::getInstance().unsetParentStatement();
    return stmtIndex;
}

void DesignExtractor::handleCondition(Condition *condition) {
    switch (condition->getConditionType()) {
    case ConditionType::SINGLE:
        handleSingleCondition(dynamic_cast<SingleCondition *>(condition));
    case ConditionType::AND:
    case ConditionType::OR:
        handleBinaryCondition(condition);
    case ConditionType::NOT:
        handleNotCondition(dynamic_cast<NotCondition *>(condition));
    default:
        throw runtime_error("Invalid ConditionType.");
    }
}

void DesignExtractor::handleSingleCondition(SingleCondition *singleCondition) {
    handleRelation(singleCondition->getRelation());
}

void DesignExtractor::handleNotCondition(NotCondition *notCondition) {
    handleCondition(notCondition->getPrimaryCondition());
}

void DesignExtractor::handleBinaryCondition(Condition *binaryCondition) {
    handleCondition(binaryCondition->getPrimaryCondition());
    handleCondition(binaryCondition->getSecondaryCondition());
}

void DesignExtractor::handleRelation(Relation *relation) {
    handleFactor(relation->getLeftFactor());
    handleFactor(relation->getRightFactor());
}

void DesignExtractor::handleExpression(Expression *expression) {
    // TODO
    switch (expression->getExpressionType()) {
    case ExpressionType::SINGLE_TERM:
        handleSingleTermExpression(
            dynamic_cast<SingleTermExpression *>(expression));
        break;
    case ExpressionType::SUBTRACT_TERMS:
        // TODO: handle
    case ExpressionType::SUM_TERMS:
        // TODO: handle
    default:
        throw runtime_error("Invalid ExpressionType.");
    }
}

void DesignExtractor::handleSingleTermExpression(
    Expression *singleTermExpression) {
    // TODO
}

void DesignExtractor::handleTerm(Term *term) {
    switch (term->getTermType()) {
    case TermType::SINGLE_FACTOR:
        handleSingleFactorTerm(term);
    case TermType::MULTIPLY_TERM_BY_FACTOR:
    case TermType::DIVIDE_TERM_BY_FACTOR:
    case TermType::MODULO_TERM_BY_FACTOR:
        handleMultiFactorTerm(term);
    default:
        throw runtime_error("Invalid TermType.");
    }
}

void DesignExtractor::handleSingleFactorTerm(Term *singleFactorTerm) {
    handleFactor(singleFactorTerm->getFactor());
}

void DesignExtractor::handleMultiFactorTerm(Term *multiFactorTerm) {
    handleFactor(multiFactorTerm->getFactor());
    handleTerm(multiFactorTerm->getTerm());
}

void DesignExtractor::handleFactor(Factor *factor) {
    switch (factor->getFactorType()) {
    case FactorType::EXPRESSION:
        handleExpression(dynamic_cast<Expression *>(factor));
    case FactorType::VARIABLE:
        handleVariable(dynamic_cast<Variable *>(factor));
    case FactorType::CONSTANT:
        handleConstantValue(dynamic_cast<ConstantValue *>(factor));
    default:
        throw runtime_error("Invalid FactorType.");
    }
}

VarIndex DesignExtractor::handleVariable(Variable *variable) {
    VarIndex varIndex = pkb.insertVar(variable);

    optional<StmtIndex> usingStmtIndex =
        ExtractionContext::getInstance().getUsingStatement();
    if (usingStmtIndex.has_value()) {
        Statement *usingStmt = pkb.getStmtByIndex(usingStmtIndex.value());
        pkb.insertStmtUsingVar(usingStmt, variable);
    }

    optional<StmtIndex> modifyingStmtIndex =
        ExtractionContext::getInstance().getModifyingStatement();
    if (modifyingStmtIndex.has_value()) {
        Statement *modifyingStmt =
            pkb.getStmtByIndex(modifyingStmtIndex.value());
        pkb.insertStmtModifyingVar(modifyingStmt, variable);
    }
    return varIndex;
}

ConstIndex DesignExtractor::handleConstantValue(ConstantValue *constantValue) {
    return pkb.insertConst(constantValue);
}