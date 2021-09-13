#include "source_processor/design_extractor/DesignExtractor.h"

using namespace std;

DesignExtractor::DesignExtractor(PKB *pkb) : pkb(pkb) {}

void DesignExtractor::extract(Program program) {
    extractDepthFirst(program);
    extractBreadthFirst(program);
}

void DesignExtractor::extractDepthFirst(Program program) {
    for (Procedure proc : program.getProcLst()) {
        extractProcedure(proc);
    }
}

void DesignExtractor::extractBreadthFirst(Program program) {
    FollowsExtractor extractor(pkb);
    extractor.extract(std::move(program));
}

void DesignExtractor::extractProcedure(Procedure procedure) {
    ExtractionContext::getInstance().getProcedureContext().push(&procedure);
    pkb->insertProc(&procedure);
    for (Statement statement : procedure.getStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().getProcedureContext().pop(&procedure);
}

StmtIndex DesignExtractor::extractStatement(Statement statement) {
    StmtIndex stmtIndex;
    switch (statement->getStatementType()) {
    case StatementType::ASSIGN:
        stmtIndex = extractAssignStatement(statement);
        break;
    case StatementType::CALL:
        stmtIndex = extractCallStatement(statement);
        break;
    case StatementType::IF:
        stmtIndex = extractIfStatement(statement);
        break;
    case StatementType::PRINT:
        stmtIndex = extractPrintStatement(statement);
        break;
    case StatementType::READ:
        stmtIndex = extractReadStatement(statement);
        break;
    case StatementType::WHILE:
        stmtIndex = extractWhileStatement(statement);
        break;
    default:
        throw runtime_error("Invalid StatementType!");
    }
    return stmtIndex;
}

StmtIndex DesignExtractor::extractAssignStatement(Statement assignStatement) {
    StmtIndex stmtIndex = pkb->insertStmt(&assignStatement);

    // Handle LHS
    ExtractionContext::getInstance().setModifyingStatement(&assignStatement);
    extractVariable(assignStatement.getVariable());
    extractModifiesRelationship(assignStatement.getVariable());
    ExtractionContext::getInstance().unsetModifyingStatement(&assignStatement);

    // Handle RHS
    ExtractionContext::getInstance().setUsingStatement(&assignStatement);
    extractExpression(assignStatement.getExpression());
    ExtractionContext::getInstance().unsetUsingStatement(&assignStatement);

    return stmtIndex;
}

StmtIndex DesignExtractor::extractCallStatement(Statement callStatement) {
    StmtIndex stmtIndex = pkb->insertStmt(&callStatement);

    // TODO: Get actual proc
    ProcName procName = callStatement.getProcName();
    ExtractionContext::getInstance()
        /// At this point, we segue into extracting the called Procedure
        /// so that Procedures are guaranteed to be extracted before their
        /// corresponding Call statements to ensure transitivity is handled
        /// properly.
        /// NOTE: This will eventually bottom out since we are guaranteed
        /// there are no recursive calls in SIMPLE.
        /// TODO:

        set<VarName>
            modifiedVarNames = pkb->getVarsModifiedByProc(procedure.getName());
    if (!modifiedVarNames.empty()) {
        for (VarName modifiedVarName : modifiedVarNames) {
            Variable *modifiedVar = pkb->getVarByName(modifiedVarName);
            extractModifiesRelationship(*modifiedVar);
        }
    }

    set<VarName> usedVarNames = pkb->getVarsUsedByProc(procedure.getName());
    if (!usedVarNames.empty()) {
        for (VarName usedVarName : usedVarNames) {
            Variable *usedVar = pkb->getVarByName(usedVarName);
            extractUsesRelationship(*usedVar);
        }
    }

    return stmtIndex;
}

StmtIndex DesignExtractor::extractIfStatement(Statement ifStatement) {
    // 0. Insert statement into PKB
    StmtIndex stmtIndex = pkb->insertStmt(&ifStatement);

    // 1. Handle condition
    ExtractionContext::getInstance().setUsingStatement(&ifStatement);
    extractCondition(ifStatement.getCondition());
    ExtractionContext::getInstance().unsetUsingStatement(&ifStatement);

    ExtractionContext::getInstance().getParentContext().push(&ifStatement);
    // 2. Handle THEN statements
    for (Statement statement : ifStatement.getThenStmtLst()) {
        extractStatement(statement);
    }

    // 3. Handle ELSE statements
    for (Statement statement : ifStatement.getElseStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().getParentContext().pop(&ifStatement);
    return stmtIndex;
}

StmtIndex DesignExtractor::extractReadStatement(Statement readStatement) {
    StmtIndex stmtIndex = pkb->insertStmt(&readStatement);
    ExtractionContext::getInstance().setModifyingStatement(&readStatement);
    extractVariable(readStatement.getVariable());
    ExtractionContext::getInstance().unsetModifyingStatement(&readStatement);
    return stmtIndex;
}

StmtIndex DesignExtractor::extractPrintStatement(Statement printStatement) {
    StmtIndex stmtIndex = pkb->insertStmt(&printStatement);
    ExtractionContext::getInstance().setUsingStatement(&printStatement);
    extractVariable(printStatement.getVariable());
    ExtractionContext::getInstance().unsetUsingStatement(&printStatement);
    return stmtIndex;
}

StmtIndex DesignExtractor::extractWhileStatement(Statement whileStatement) {
    // 0. Insert statement into PKB
    StmtIndex stmtIndex = pkb->insertStmt(&whileStatement);

    // 1. Handle condition
    ExtractionContext::getInstance().setUsingStatement(&whileStatement);
    extractCondition(whileStatement.getCondition());
    ExtractionContext::getInstance().unsetUsingStatement(&whileStatement);

    // 2. Handle THEN statements
    ExtractionContext::getInstance().getParentContext().push(&whileStatement);
    for (Statement statement : whileStatement.getThenStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().getParentContext().pop(&whileStatement);

    return stmtIndex;
}

void DesignExtractor::extractCondition(Condition condition) {
    switch (condition->getConditionType()) {
    case ConditionType::SINGLE:
        extractSingleCondition(condition);
    case ConditionType::AND:
    case ConditionType::OR:
        extractBinaryCondition(condition);
    case ConditionType::NOT:
        extractNotCondition(condition);
    default:
        throw runtime_error("Invalid ConditionType.");
    }
}

void DesignExtractor::extractSingleCondition(Condition singleCondition) {
    extractRelation(singleCondition.getRelation());
}

void DesignExtractor::extractNotCondition(Condition notCondition) {
    extractCondition(notCondition.getPrimaryCondition());
}

void DesignExtractor::extractBinaryCondition(Condition binaryCondition) {
    extractCondition(binaryCondition.getPrimaryCondition());
    extractCondition(binaryCondition.getSecondaryCondition());
}

void DesignExtractor::extractRelation(Relation relation) {
    extractFactor(relation.getLeftFactor());
    extractFactor(relation.getRightFactor());
}

void DesignExtractor::extractExpression(Expression expression) {
    switch (expression.getExpressionType()) {
    case ExpressionType::SINGLE_TERM:
        extractSingleTermExpression(expression);
        break;
    case ExpressionType::SUBTRACT_TERMS:
        extractSubtractTermsExpression(expression);
        break;
    case ExpressionType::SUM_TERMS:
        extractSumTermsExpression(expression);
        break;
    default:
        throw runtime_error("Invalid ExpressionType.");
    }
}

void DesignExtractor::extractSingleTermExpression(
    Expression singleTermExpression) {
    extractTerm(singleTermExpression.getTerm());
}

void DesignExtractor::extractSubtractTermsExpression(
    Expression subtractTermsExpression) {
    extractExpression(subtractTermsExpression.getExpression());
    extractTerm(subtractTermsExpression.getTerm());
}

void DesignExtractor::extractSumTermsExpression(Expression sumTermsExpression) {
    extractExpression(sumTermsExpression.getExpression());
    extractTerm(sumTermsExpression.getTerm());
}

void DesignExtractor::extractTerm(Term term) {
    switch (term.getTermType()) {
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

void DesignExtractor::extractSingleFactorTerm(Term singleFactorTerm) {
    extractFactor(singleFactorTerm.getFactor());
}

void DesignExtractor::extractMultiFactorTerm(Term multiFactorTerm) {
    extractFactor(multiFactorTerm.getFactor());
    extractTerm(multiFactorTerm.getTerm());
}

void DesignExtractor::extractFactor(Factor factor) {
    switch (factor.getFactorType()) {
    case FactorType::EXPRESSION:
        extractExpression(factor);
    case FactorType::VARIABLE:
        extractVariable(factor);
    case FactorType::CONSTANT:
        extractConstantValue(factor);
    default:
        throw runtime_error("Invalid FactorType.");
    }
}

void DesignExtractor::extractVariable(Variable variable) {
    pkb->insertVar(&variable);
    extractUsesRelationship(variable);
    extractModifiesRelationship(variable);
}

void DesignExtractor::extractUsesRelationship(Variable variable) {
    optional<Statement *> usingStatement =
        ExtractionContext::getInstance().getUsingStatement();
    if (!usingStatement.has_value()) {
        return;
    }

    // 1. Handle using statement
    pkb->insertStmtUsingVar(usingStatement.value(), &variable);

    // 2. Handle all parent statements
    vector<Statement *> parentStatements =
        ExtractionContext::getInstance().getParentContext().getAllEntities();
    if (!parentStatements.empty()) {
        for (Statement *parentStatement : parentStatements) {
            pkb->insertStmtModifyingVar(parentStatement, &variable);
        }
    }

    // 3. Handle all enclosing procedures
    vector<Procedure *> usingProcedures =
        ExtractionContext::getInstance().getProcedureContext().getAllEntities();
    if (!usingProcedures.empty()) {
        for (Procedure *usingProcedure : usingProcedures) {
            pkb->insertProcUsingVar(usingProcedure, &variable);
        }
    }
}

void DesignExtractor::extractModifiesRelationship(Variable variable) {
    optional<Statement *> modifyingStatement =
        ExtractionContext::getInstance().getModifyingStatement();
    if (!modifyingStatement.has_value()) {
        return;
    }

    // 1. Handle modifying statement
    pkb->insertStmtModifyingVar(modifyingStatement.value(), &variable);

    // 2. Handle all parent statements
    vector<Statement *> parentStatements =
        ExtractionContext::getInstance().getParentContext().getAllEntities();
    if (!parentStatements.empty()) {
        for (Statement *parentStatement : parentStatements) {
            pkb->insertStmtModifyingVar(parentStatement, &variable);
        }
    }

    // 3. Handle all enclosing procedures
    vector<Procedure *> modifyingProcedures =
        ExtractionContext::getInstance().getProcedureContext().getAllEntities();
    if (!modifyingProcedures.empty()) {
        for (Procedure *modifyingProcedure : modifyingProcedures) {
            pkb->insertProcModifyingVar(modifyingProcedure, &variable);
        }
    }
}

void DesignExtractor::extractConstantValue(ConstantValue constantValue) {
    pkb->insertConst(&constantValue);
}
