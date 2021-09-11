#include "source_processor/DesignExtractor.h"

using namespace std;

DesignExtractor::DesignExtractor(PKB *pkb) : pkb(pkb) {}

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
    ProcIndex procIndex = pkb->insertProc(procedure);
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
        stmtIndex =
            extractAssignStatement(dynamic_cast<AssignStatement *>(statement));
        break;
    case StatementType::CALL:
        stmtIndex =
            extractCallStatement(dynamic_cast<CallStatement *>(statement));
        break;
    case StatementType::IF:
        stmtIndex = extractIfStatement(dynamic_cast<IfStatement *>(statement));
        break;
    case StatementType::PRINT:
        stmtIndex =
            extractPrintStatement(dynamic_cast<PrintStatement *>(statement));
        break;
    case StatementType::READ:
        stmtIndex =
            extractReadStatement(dynamic_cast<ReadStatement *>(statement));
        break;
    case StatementType::WHILE:
        stmtIndex =
            extractWhileStatement(dynamic_cast<WhileStatement *>(statement));
        break;
    default:
        throw runtime_error("Invalid StatementType!");
    }
    return stmtIndex;
}

StmtIndex
DesignExtractor::extractAssignStatement(AssignStatement *assignStatement) {
    StmtIndex stmtIndex = pkb->insertStmt(assignStatement);

    // Handle LHS
    ExtractionContext::getInstance().setModifyingStatement(assignStatement);
    extractVariable(assignStatement->getVariable());
    extractModifiesRelationship(assignStatement->getVariable());
    ExtractionContext::getInstance().unsetModifyingStatement(assignStatement);

    // Handle RHS
    ExtractionContext::getInstance().setUsingStatement(assignStatement);
    extractExpression(assignStatement->getExpression());
    ExtractionContext::getInstance().unsetUsingStatement(assignStatement);

    return stmtIndex;
}

StmtIndex DesignExtractor::extractCallStatement(CallStatement *callStatement) {
    StmtIndex stmtIndex = pkb->insertStmt(callStatement);

    /// TODO: Topologically sort all Procedures, so that Procedures are
    /// guaranteed to be extracted before they are called,
    /// and call statements are transitively handled

    Procedure procedure = callStatement->getProcedure();

    set<VarName> modifiedVarNames =
        pkb->getVarsModifiedByProc(procedure.getName());
    if (!modifiedVarNames.empty()) {
        for (VarName modifiedVarName : modifiedVarNames) {
            Variable *modifiedVar = pkb->getVarByName(modifiedVarName);
            extractModifiesRelationship(modifiedVar);
        }
    }

    set<VarName> usedVarNames = pkb->getVarsUsedByProc(procedure.getName());
    if (!usedVarNames.empty()) {
        for (VarName usedVarName : usedVarNames) {
            Variable *usedVar = pkb->getVarByName(usedVarName);
            extractUsesRelationship(usedVar);
        }
    }

    return stmtIndex;
}

StmtIndex DesignExtractor::extractIfStatement(IfStatement *ifStatement) {
    // 0. Insert statement into PKB
    StmtIndex stmtIndex = pkb->insertStmt(ifStatement);

    // 1. Handle condition
    ExtractionContext::getInstance().setUsingStatement(ifStatement);
    extractCondition(ifStatement->getCondition());
    ExtractionContext::getInstance().unsetUsingStatement(ifStatement);

    ExtractionContext::getInstance().getParentContext().push(ifStatement);
    // 2. Handle THEN statements
    for (Statement *statement : ifStatement->getThenStmtLst()) {
        extractStatement(statement);
    }

    // 3. Handle ELSE statements
    for (Statement *statement : ifStatement->getElseStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().getParentContext().pop(ifStatement);
    return stmtIndex;
}

StmtIndex DesignExtractor::extractReadStatement(ReadStatement *readStatement) {
    StmtIndex stmtIndex = pkb->insertStmt(readStatement);
    ExtractionContext::getInstance().setModifyingStatement(readStatement);
    extractVariable(readStatement->getVariable());
    ExtractionContext::getInstance().unsetModifyingStatement(readStatement);
    return stmtIndex;
}

StmtIndex
DesignExtractor::extractPrintStatement(PrintStatement *printStatement) {
    StmtIndex stmtIndex = pkb->insertStmt(printStatement);
    ExtractionContext::getInstance().unsetUsingStatement(printStatement);
    extractVariable(printStatement->getVariable());
    ExtractionContext::getInstance().unsetUsingStatement(printStatement);
    return stmtIndex;
}

StmtIndex
DesignExtractor::extractWhileStatement(WhileStatement *whileStatement) {
    // 0. Insert statement into PKB
    StmtIndex stmtIndex = pkb->insertStmt(whileStatement);

    // 1. Handle condition
    ExtractionContext::getInstance().setUsingStatement(whileStatement);
    extractCondition(whileStatement->getCondition());
    ExtractionContext::getInstance().unsetUsingStatement(whileStatement);

    // 2. Handle THEN statements
    ExtractionContext::getInstance().getParentContext().push(whileStatement);
    for (Statement *statement : whileStatement->getThenStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().getParentContext().pop(whileStatement);

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
    VarIndex varIndex = pkb->insertVar(variable);
    extractUsesRelationship(variable);
    extractModifiesRelationship(variable);
    return varIndex;
}

void DesignExtractor::extractUsesRelationship(Variable *variable) {
    optional<Statement *> usingStatement =
        ExtractionContext::getInstance().getUsingStatement();
    if (!usingStatement.has_value()) {
        return;
    }

    // 1. Handle using statement
    pkb->insertStmtUsingVar(usingStatement.value(), variable);

    // 2. Handle all parent statements
    vector<Statement *> parentStatements =
        ExtractionContext::getInstance().getParentContext().getAllEntities();
    if (!parentStatements.empty()) {
        for (Statement *parentStatement : parentStatements) {
            pkb->insertStmtModifyingVar(parentStatement, variable);
        }
    }

    // 3. Handle all enclosing procedures
    vector<Procedure *> usingProcedures =
        ExtractionContext::getInstance().getProcedureContext().getAllEntities();
    if (!usingProcedures.empty()) {
        for (Procedure *usingProcedure : usingProcedures) {
            pkb->insertProcUsingVar(usingProcedure, variable);
        }
    }
}

void DesignExtractor::extractModifiesRelationship(Variable *variable) {
    optional<Statement *> modifyingStatement =
        ExtractionContext::getInstance().getModifyingStatement();
    if (!modifyingStatement.has_value()) {
        return;
    }

    // 1. Handle modifying statement
    pkb->insertStmtModifyingVar(modifyingStatement.value(), variable);

    // 2. Handle all parent statements
    vector<Statement *> parentStatements =
        ExtractionContext::getInstance().getParentContext().getAllEntities();
    if (!parentStatements.empty()) {
        for (Statement *parentStatement : parentStatements) {
            pkb->insertStmtModifyingVar(parentStatement, variable);
        }
    }

    // 3. Handle all enclosing procedures
    vector<Procedure *> modifyingProcedures =
        ExtractionContext::getInstance().getProcedureContext().getAllEntities();
    if (!modifyingProcedures.empty()) {
        for (Procedure *modifyingProcedure : modifyingProcedures) {
            pkb->insertProcModifyingVar(modifyingProcedure, variable);
        }
    }
}

ConstIndex DesignExtractor::extractConstantValue(ConstantValue *constantValue) {
    return pkb->insertConst(constantValue);
}