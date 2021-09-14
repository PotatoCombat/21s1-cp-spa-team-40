#pragma once

#include "ExtractionContext.h"
#include "FollowsExtractor.h"
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Statement.h"
#include "pkb/PKB.h"

class DesignExtractor {
public:
    explicit DesignExtractor(PKB *pkb);

    // Program
    void extract(Program *program);
    void extractDepthFirst(Program *program);
    void extractBreadthFirst(Program *program);

    // Procedure
    void extractProcedure(Procedure *program);

    // Statement
    StmtIndex extractStatement(Statement *statement);
    void extractAssignStatement(Statement *assignStatement);
    void extractCallStatement(Statement *callStatement);
    void extractIfStatement(Statement *ifStatement);
    void extractPrintStatement(Statement *printStatement);
    void extractReadStatement(Statement *readStatement);
    void extractWhileStatement(Statement *whileStatement);

    // Variable
    void extractVariable(Variable *variable);
    void extractModifiesRelationship(Variable *variable);
    void extractUsesRelationship(Variable *variable);

    // Condition
    //    void extractCondition(Condition condition);
    //    void extractSingleCondition(Condition singleCondition);
    //    void extractNotCondition(Condition notCondition);
    //    void extractBinaryCondition(Condition binaryCondition);

    // Relation
    //    void extractRelation(Relation relation);

    // Expression
    //    void extractExpression(Expression expression);
    //    void extractSingleTermExpression(Expression singleTermExpression);
    //    void extractSubtractTermsExpression(Expression
    //    subtractTermsExpression); void extractSumTermsExpression(Expression
    //    sumTermsExpression);

    // Term
    //    void extractTerm(Term term);
    //    void extractSingleFactorTerm(Term singleFactorTerm);
    //    void extractMultiFactorTerm(Term multiFactorTerm);

    // Factor
    //    void extractFactor(Factor factor);

    // ConstantValue
    //    void extractConstantValue(ConstantValue constantValue);

private:
    PKB *pkb;
};
