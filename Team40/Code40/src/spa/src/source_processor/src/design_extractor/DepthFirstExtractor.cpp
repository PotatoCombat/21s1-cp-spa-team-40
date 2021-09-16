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
    pkb->insertProc(procedure);
    for (Statement *statement : procedure->getStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().unsetCurrentProcedure(procedure);
}

void DepthFirstExtractor::extractStatement(Statement *statement) {
    pkb->insertStmt(statement);
    vector<Statement *> parentStatement =
        ExtractionContext::getInstance().getParentStatements();
    if (!parentStatement.empty()) {
        pkb->insertParent(parentStatement.back(), statement);
    }
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
}

void DepthFirstExtractor::extractCallStatement(Statement *callStatement) {

    ProcName calledProcName = callStatement->getProcName();

    optional<Procedure *> currentProcedure =
        ExtractionContext::getInstance().getCurrentProcedure();
    if (!currentProcedure.has_value()) {
        throw runtime_error("Current procedure not set.");
    }
    ExtractionContext::getInstance().addProcDependency(
        currentProcedure.value()->getName(), calledProcName);
}

void DepthFirstExtractor::extractIfStatement(Statement *ifStatement) {
    // 1. Handle condition
    ExtractionContext::getInstance().setUsingStatement(ifStatement);
    for (Variable *variable : ifStatement->getExpressionVars()) {
        extractVariable(variable);
    }
    for (ConstantValue *constantValue : ifStatement->getExpressionConsts()) {
        extractConstantValue(constantValue);
    }
    ExtractionContext::getInstance().unsetUsingStatement(ifStatement);

    // 2. Handle THEN statements
    ExtractionContext::getInstance().setParentStatement(ifStatement);
    for (Statement *statement : ifStatement->getThenStmtLst()) {
        extractStatement(statement);
    }

    // 3. Handle ELSE statements
    for (Statement *statement : ifStatement->getElseStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().unsetParentStatement(ifStatement);
}

void DepthFirstExtractor::extractReadStatement(Statement *readStatement) {
    ExtractionContext::getInstance().setModifyingStatement(readStatement);
    extractVariable(readStatement->getVariable());
    ExtractionContext::getInstance().unsetModifyingStatement(readStatement);
}

void DepthFirstExtractor::extractPrintStatement(Statement *printStatement) {
    ExtractionContext::getInstance().setUsingStatement(printStatement);
    extractVariable(printStatement->getVariable());
    ExtractionContext::getInstance().unsetUsingStatement(printStatement);
}

void DepthFirstExtractor::extractWhileStatement(Statement *whileStatement) {
    // 1. Handle condition
    ExtractionContext::getInstance().setUsingStatement(whileStatement);
    for (Variable *variable : whileStatement->getExpressionVars()) {
        extractVariable(variable);
    }
    for (ConstantValue *constantValue : whileStatement->getExpressionConsts()) {
        extractConstantValue(constantValue);
    }
    ExtractionContext::getInstance().unsetUsingStatement(whileStatement);

    // 2. Handle THEN statements
    ExtractionContext::getInstance().setParentStatement(whileStatement);
    for (Statement *statement : whileStatement->getThenStmtLst()) {
        extractStatement(statement);
    }
    ExtractionContext::getInstance().unsetParentStatement(whileStatement);
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

/**
 * The following methods are not used in iteration 1,
 * but are likely relevant in the future. Do not remove.
 */

// void DepthFirstExtractor::extractCondition(Condition condition) {
//     switch (condition.getConditionType()) {
//     case ConditionType::SINGLE:
//         extractSingleCondition(condition);
//     case ConditionType::AND:
//     case ConditionType::OR:
//         extractBinaryCondition(condition);
//     case ConditionType::NOT:
//         extractNotCondition(condition);
//     default:
//         throw runtime_error("Invalid ConditionType.");
//     }
// }

// void DepthFirstExtractor::extractSingleCondition(Condition singleCondition) {
//     extractRelation(singleCondition.getRelation());
// }
//
// void DepthFirstExtractor::extractNotCondition(Condition notCondition) {
//     extractCondition(notCondition.getPrimaryCondition());
// }
//
// void DepthFirstExtractor::extractBinaryCondition(Condition binaryCondition) {
//     extractCondition(binaryCondition.getPrimaryCondition());
//     extractCondition(binaryCondition.getSecondaryCondition());
// }
//
// void DepthFirstExtractor::extractRelation(Relation relation) {
//     extractFactor(relation.getLeftFactor());
//     extractFactor(relation.getRightFactor());
// }
//
// void DepthFirstExtractor::extractExpression(Expression expression) {
//     switch (expression.getExpressionType()) {
//     case ExpressionType::SINGLE_TERM:
//         extractSingleTermExpression(expression);
//         break;
//     case ExpressionType::SUBTRACT_TERMS:
//         extractSubtractTermsExpression(expression);
//         break;
//     case ExpressionType::SUM_TERMS:
//         extractSumTermsExpression(expression);
//         break;
//     default:
//         throw runtime_error("Invalid ExpressionType.");
//     }
// }
//
// void DepthFirstExtractor::extractSingleTermExpression(
//     Expression singleTermExpression) {
//     extractTerm(singleTermExpression.getTerm());
// }
//
// void DepthFirstExtractor::extractSubtractTermsExpression(
//     Expression subtractTermsExpression) {
//     extractExpression(subtractTermsExpression.getExpression());
//     extractTerm(subtractTermsExpression.getTerm());
// }
//
// void DepthFirstExtractor::extractSumTermsExpression(Expression
// sumTermsExpression) {
//     extractExpression(sumTermsExpression.getExpression());
//     extractTerm(sumTermsExpression.getTerm());
// }
//
// void DepthFirstExtractor::extractTerm(Term term) {
//     switch (term.getTermType()) {
//     case TermType::SINGLE_FACTOR:
//         extractSingleFactorTerm(term);
//     case TermType::MULTIPLY_TERM_BY_FACTOR:
//     case TermType::DIVIDE_TERM_BY_FACTOR:
//     case TermType::MODULO_TERM_BY_FACTOR:
//         extractMultiFactorTerm(term);
//     default:
//         throw runtime_error("Invalid TermType.");
//     }
// }
//
// void DepthFirstExtractor::extractSingleFactorTerm(Term singleFactorTerm) {
//     extractFactor(singleFactorTerm.getFactor());
// }
//
// void DepthFirstExtractor::extractMultiFactorTerm(Term multiFactorTerm) {
//     extractFactor(multiFactorTerm.getFactor());
//     extractTerm(multiFactorTerm.getTerm());
// }
//
// void DepthFirstExtractor::extractFactor(Factor factor) {
//     switch (factor.getFactorType()) {
//     case FactorType::EXPRESSION:
//         extractExpression(factor);
//     case FactorType::VARIABLE:
//         extractVariable(factor);
//     case FactorType::CONSTANT:
//         extractConstantValue(factor);
//     default:
//         throw runtime_error("Invalid FactorType.");
//     }
// }
//
