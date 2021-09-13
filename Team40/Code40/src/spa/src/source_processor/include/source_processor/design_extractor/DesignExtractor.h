#pragma once

#include "ExtractionContext.h"
#include "FollowsExtractor.h"
#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/condition/NotCondition.h"
#include "common/model/condition/SingleCondition.h"
#include "common/model/expression/SingleTermExpression.h"
#include "common/model/expression/SubtractTermsExpression.h"
#include "common/model/expression/SumTermsExpression.h"
#include "common/model/statement/AssignStatement.h"
#include "common/model/statement/CallStatement.h"
#include "common/model/statement/IfStatement.h"
#include "common/model/statement/PrintStatement.h"
#include "common/model/statement/ReadStatement.h"
#include "common/model/statement/Statement.h"
#include "common/model/statement/WhileStatement.h"
#include "common/model/term/SingleFactorTerm.h"
#include "common/model/term/Term.h"
#include "pkb/PKB.h"

class DesignExtractor {
public:
    explicit DesignExtractor(PKB *pkb);

    // Program
    void extract(Program program);
    void extractDepthFirst(Program program);
    void extractBreadthFirst(Program program);

    // Procedure
    void extractProcedure(Procedure program);

    // Statement
    StmtIndex extractStatement(Statement statement);
    StmtIndex extractAssignStatement(Statement assignStatement);
    StmtIndex extractCallStatement(Statement callStatement);
    StmtIndex extractIfStatement(Statement ifStatement);
    StmtIndex extractPrintStatement(Statement printStatement);
    StmtIndex extractReadStatement(Statement readStatement);
    StmtIndex extractWhileStatement(Statement whileStatement);

    // Condition
    void extractCondition(Condition condition);
    void extractSingleCondition(Condition singleCondition);
    void extractNotCondition(Condition notCondition);
    void extractBinaryCondition(Condition binaryCondition);

    // Relation
    void extractRelation(Relation relation);

    // Expression
    void extractExpression(Expression expression);
    void extractSingleTermExpression(Expression singleTermExpression);
    void extractSubtractTermsExpression(Expression subtractTermsExpression);
    void extractSumTermsExpression(Expression sumTermsExpression);

    // Term
    void extractTerm(Term term);
    void extractSingleFactorTerm(Term singleFactorTerm);
    void extractMultiFactorTerm(Term multiFactorTerm);

    // Factor
    void extractFactor(Factor factor);

    // Variable
    void extractVariable(Variable variable);
    void extractModifiesRelationship(Variable variable);
    void extractUsesRelationship(Variable variable);

    // ConstantValue
    void extractConstantValue(ConstantValue constantValue);

private:
    PKB *pkb;
};
