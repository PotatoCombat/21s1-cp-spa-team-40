#include "source_processor/DesignExtractor.h"

using namespace std;

DesignExtractor::DesignExtractor(PKB pkb) : pkb(pkb) {}

void DesignExtractor::extractEntities(Program program) {
    for (Procedure proc : program.getProcLst()) {
        handleProcedure(&proc);
    }
}

void DesignExtractor::extractRelationships(const Program &program) {
    extractFollowsRelationship(program);
    extractParentsRelationship(program);
}

void DesignExtractor::extractFollowsRelationship(Program program) {
    FollowsExtractor extractor(pkb);
    extractor.handleProgram(std::move(program));
}

void DesignExtractor::extractParentsRelationship(Program program) {
    ParentsExtractor extractor(pkb);
    extractor.handleProgram(std::move(program));
}

ProcIndex DesignExtractor::handleProcedure(Procedure *procedure) {
    ProcIndex procIndex = pkb.insertProc(procedure);
    ExtractionContext::getInstance().getProcedureContext().push(procedure);
    for (Statement *statement : procedure->getStmtLst()) {
        handleStatement(statement);
    }
    ExtractionContext::getInstance().getProcedureContext().pop(procedure);
    return procIndex;
}

StmtIndex DesignExtractor::handleStatement(Statement *statement) {
    StmtIndex stmtIndex;
    switch (statement->getStatementType()) {
    case StatementType::ASSIGN:
        stmtIndex = handleAssignStatement(statement);
        break;
    case StatementType::CALL:
        stmtIndex =
            handleCallStatement(dynamic_cast<CallStatement *>(statement));
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
    return stmtIndex;
}

StmtIndex DesignExtractor::handleAssignStatement(Statement *assignStatement) {
    StmtIndex stmtIndex = pkb.insertStmt(assignStatement);
    Variable *var = assignStatement->getVariable();
    VarIndex varIdx = pkb.insertVar(var);
    pkb.insertStmtModifyingVar(assignStatement, var);

    // Handle LHS
    ExtractionContext::getInstance().getModifiesContext().push(assignStatement);
    handleVariable(assignStatement->getVariable());
    ExtractionContext::getInstance().getModifiesContext().pop(assignStatement);

    // Handle RHS
    ExtractionContext::getInstance().getModifiesContext().push(assignStatement);
    handleExpression(assignStatement->getExpression());
    ExtractionContext::getInstance().getModifiesContext().pop(assignStatement);

    return stmtIndex;
}

StmtIndex DesignExtractor::handleCallStatement(CallStatement *callStatement) {
    // TODO: Add all modified / used vars to this statement once topo-sort of
    // Procedures is done
    return pkb.insertStmt(callStatement);
}

StmtIndex DesignExtractor::handleIfStatement(Statement *ifStatement) {
    // 0. Insert statement into PKB
    StmtIndex stmtIndex = pkb.insertStmt(ifStatement);

    // 1. Handle condition
    ExtractionContext::getInstance().getUsesContext().push(ifStatement);
    handleCondition(ifStatement->getCondition());
    ExtractionContext::getInstance().getUsesContext().pop(ifStatement);

    // 2. Handle THEN statements
    for (Statement *statement : ifStatement->getThenStmtLst()) {
        handleStatement(statement);
    }

    // 3. Handle ELSE statements
    for (Statement *statement : ifStatement->getElseStmtLst()) {
        handleStatement(statement);
    }
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

    vector<Statement *> usingStatements =
        ExtractionContext::getInstance().getUsesContext().getAllEntities();
    if (!usingStatements.empty()) {
        for (Statement *usingStatement : usingStatements) {
            pkb.insertStmtUsingVar(usingStatement, variable);
        }
    }

    vector<Statement *> modifyingStatements =
        ExtractionContext::getInstance().getModifiesContext().getAllEntities();
    if (!modifyingStatements.empty()) {
        for (Statement *modifyingStatement : modifyingStatements) {
            pkb.insertStmtModifyingVar(modifyingStatement, variable);
        }
    }
    return varIndex;
}

void DesignExtractor::handleUsesRelationship(Variable *variable) {}
ConstIndex DesignExtractor::handleConstantValue(ConstantValue *constantValue) {
    return pkb.insertConst(constantValue);
}