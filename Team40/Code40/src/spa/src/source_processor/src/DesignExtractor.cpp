#include "source_processor/DesignExtractor.h"

using namespace std;

DesignExtractor::DesignExtractor(PKB pkb) : pkb(pkb) {}

void DesignExtractor::extract(Program program) {
    extractDepthFirst(program);
    extractBreadthFirst(program);
}

void DesignExtractor::extractDepthFirst(Program program) {
    for (Procedure proc : program.getProcLst()) {
        extractProcedure(&proc);
    }
}

void DesignExtractor::extractBreadthFirst(Program program) {
    FollowsExtractor extractor(pkb);
    extractor.extract(std::move(program));
}

ProcIndex DesignExtractor::extractProcedure(Procedure *procedure) {
    ProcIndex procIndex = pkb.insertProc(procedure);
    ExtractionContext::getInstance().getProcedureContext().push(procedure);
    for (Statement *statement : procedure->getStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().getProcedureContext().pop(procedure);
    return procIndex;
}

StmtIndex DesignExtractor::extractStatement(Statement *statement) {
    StmtIndex stmtIndex;
    switch (statement->getStatementType()) {
    case StatementType::ASSIGN:
        stmtIndex = extractAssignStatement(statement);
        break;
    case StatementType::CALL:
        stmtIndex =
            extractCallStatement(dynamic_cast<CallStatement *>(statement));
        break;
    case StatementType::IF:
        stmtIndex = extractIfStatement(statement);
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

StmtIndex DesignExtractor::extractAssignStatement(Statement *assignStatement) {
    StmtIndex stmtIndex = pkb.insertStmt(assignStatement);

    // Handle LHS
    ExtractionContext::getInstance().getModifiesContext().push(assignStatement);
    extractVariable(assignStatement->getVariable());
    extractModifiesRelationship(assignStatement->getVariable());
    ExtractionContext::getInstance().getModifiesContext().pop(assignStatement);

    // Handle RHS
    ExtractionContext::getInstance().getModifiesContext().push(assignStatement);
    extractExpression(assignStatement->getExpression());
    extractUsesRelationship(assignStatement->getVariable());
    ExtractionContext::getInstance().getModifiesContext().pop(assignStatement);

    return stmtIndex;
}

StmtIndex DesignExtractor::extractCallStatement(CallStatement *callStatement) {
    // TODO: Add all modified / used vars to this statement once topo-sort of
    // Procedures is done
    return pkb.insertStmt(callStatement);
}

StmtIndex DesignExtractor::extractIfStatement(Statement *ifStatement) {
    // 0. Insert statement into PKB
    StmtIndex stmtIndex = pkb.insertStmt(ifStatement);

    // 1. Handle condition
    ExtractionContext::getInstance().getUsesContext().push(ifStatement);
    extractCondition(ifStatement->getCondition());
    ExtractionContext::getInstance().getUsesContext().pop(ifStatement);

    // 2. Handle THEN statements
    for (Statement *statement : ifStatement->getThenStmtLst()) {
        extractStatement(statement);
    }

    // 3. Handle ELSE statements
    for (Statement *statement : ifStatement->getElseStmtLst()) {
        extractStatement(statement);
    }
    return stmtIndex;
}

void DesignExtractor::extractCondition(Condition *condition) {
    switch (condition->getConditionType()) {
    case ConditionType::SINGLE:
        extractSingleCondition(dynamic_cast<SingleCondition *>(condition));
    case ConditionType::AND:
    case ConditionType::OR:
        extractBinaryCondition(condition);
    case ConditionType::NOT:
        extractNotCondition(dynamic_cast<NotCondition *>(condition));
    default:
        throw runtime_error("Invalid ConditionType.");
    }
}

void DesignExtractor::extractSingleCondition(SingleCondition *singleCondition) {
    extractRelation(singleCondition->getRelation());
}

void DesignExtractor::extractNotCondition(NotCondition *notCondition) {
    extractCondition(notCondition->getPrimaryCondition());
}

void DesignExtractor::extractBinaryCondition(Condition *binaryCondition) {
    extractCondition(binaryCondition->getPrimaryCondition());
    extractCondition(binaryCondition->getSecondaryCondition());
}

void DesignExtractor::extractRelation(Relation *relation) {
    extractFactor(relation->getLeftFactor());
    extractFactor(relation->getRightFactor());
}

void DesignExtractor::extractExpression(Expression *expression) {
    switch (expression->getExpressionType()) {
    case ExpressionType::SINGLE_TERM:
        extractSingleTermExpression(
            dynamic_cast<SingleTermExpression *>(expression));
        break;
    case ExpressionType::SUBTRACT_TERMS:
        extractSubtractTermsExpression(
            dynamic_cast<SubtractTermsExpression *>(expression));
        break;
    case ExpressionType::SUM_TERMS:
        extractSumTermsExpression(
            dynamic_cast<SumTermsExpression *>(expression));
        break;
    default:
        throw runtime_error("Invalid ExpressionType.");
    }
}

void DesignExtractor::extractSingleTermExpression(
    SingleTermExpression *singleTermExpression) {
    extractTerm(singleTermExpression->getTerm());
}

void DesignExtractor::extractSubtractTermsExpression(
    SubtractTermsExpression *subtractTermsExpression) {
    extractExpression(subtractTermsExpression->getExpression());
    extractTerm(subtractTermsExpression->getTerm());
}

void DesignExtractor::extractSumTermsExpression(
    SumTermsExpression *sumTermsExpression) {
    extractExpression(sumTermsExpression->getExpression());
    extractTerm(sumTermsExpression->getTerm());
}

void DesignExtractor::extractTerm(Term *term) {
    switch (term->getTermType()) {
    case TermType::SINGLE_FACTOR:
        extractSingleFactorTerm(term);
    case TermType::MULTIPLY_TERM_BY_FACTOR:
    case TermType::DIVIDE_TERM_BY_FACTOR:
    case TermType::MODULO_TERM_BY_FACTOR:
        extractMultiFactorTerm(term);
    default:
        throw runtime_error("Invalid TermType.");
    }
}

void DesignExtractor::extractSingleFactorTerm(Term *singleFactorTerm) {
    extractFactor(singleFactorTerm->getFactor());
}

void DesignExtractor::extractMultiFactorTerm(Term *multiFactorTerm) {
    extractFactor(multiFactorTerm->getFactor());
    extractTerm(multiFactorTerm->getTerm());
}

void DesignExtractor::extractFactor(Factor *factor) {
    switch (factor->getFactorType()) {
    case FactorType::EXPRESSION:
        extractExpression(dynamic_cast<Expression *>(factor));
    case FactorType::VARIABLE:
        extractVariable(dynamic_cast<Variable *>(factor));
    case FactorType::CONSTANT:
        extractConstantValue(dynamic_cast<ConstantValue *>(factor));
    default:
        throw runtime_error("Invalid FactorType.");
    }
}

VarIndex DesignExtractor::extractVariable(Variable *variable) {
    VarIndex varIndex = pkb.insertVar(variable);
    return varIndex;
}

void DesignExtractor::extractUsesRelationship(Variable *variable) {
    vector<Statement *> usingStatements =
        ExtractionContext::getInstance().getUsesContext().getAllEntities();
    if (!usingStatements.empty()) {
        for (Statement *usingStatement : usingStatements) {
            pkb.insertStmtUsingVar(usingStatement, variable);
        }
    }
    vector<Procedure *> usingProcedures =
        ExtractionContext::getInstance().getProcedureContext().getAllEntities();
    if (!usingProcedures.empty()) {
        for (Procedure *usingProcedure : usingProcedures) {
            pkb.insertProcUsingVar(usingProcedure, variable);
        }
    }
}

void DesignExtractor::extractModifiesRelationship(Variable *variable) {
    vector<Statement *> modifyingStatements =
        ExtractionContext::getInstance().getModifiesContext().getAllEntities();
    if (!modifyingStatements.empty()) {
        for (Statement *modifyingStatement : modifyingStatements) {
            pkb.insertStmtModifyingVar(modifyingStatement, variable);
        }
    }

    vector<Procedure *> modifyingProcedures =
        ExtractionContext::getInstance().getProcedureContext().getAllEntities();
    if (!modifyingProcedures.empty()) {
        for (Procedure *modifyingProcedure : modifyingProcedures) {
            pkb.insertProcModifyingVar(modifyingProcedure, variable);
        }
    }
}

ConstIndex DesignExtractor::extractConstantValue(ConstantValue *constantValue) {
    return pkb.insertConst(constantValue);
}