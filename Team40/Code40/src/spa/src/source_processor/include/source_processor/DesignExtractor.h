#pragma once

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
#include "common/model/term/Term.h"
#include "pkb/PKB.h"
#include "source_processor/ExtractionContext.h"
#include "source_processor/FollowsExtractor.h"

class DesignExtractor {
    PKB pkb;

public:
    explicit DesignExtractor(PKB pkb);

    // Program
    void extract(Program program);
    void extractDepthFirst(Program program);
    void extractBreadthFirst(Program program);

    // Procedure
    ProcIndex extractProcedure(Procedure *program);

    // Statement
    StmtIndex extractStatement(Statement *statement);
    StmtIndex extractAssignStatement(AssignStatement *assignStatement);
    StmtIndex extractCallStatement(CallStatement *callStatement);
    StmtIndex extractIfStatement(IfStatement *ifStatement);
    StmtIndex extractPrintStatement(PrintStatement *printStatement);
    StmtIndex extractReadStatement(ReadStatement *readStatement);
    StmtIndex extractWhileStatement(WhileStatement *whileStatement);

    // Condition
    void extractCondition(Condition *condition);
    void extractSingleCondition(SingleCondition *singleCondition);
    void extractNotCondition(NotCondition *notCondition);
    void extractBinaryCondition(Condition *binaryCondition);

    // Relation
    void extractRelation(Relation *relation);

    // Expression
    void extractExpression(Expression *expression);
    void
    extractSingleTermExpression(SingleTermExpression *singleTermExpression);
    void extractSubtractTermsExpression(
        SubtractTermsExpression *subtractTermsExpression);
    void extractSumTermsExpression(SumTermsExpression *sumTermsExpression);

    // Term
    void extractTerm(Term *term);
    void extractSingleFactorTerm(Term *singleFactorTerm);
    void extractMultiFactorTerm(Term *multiFactorTerm);

    // Factor
    void extractFactor(Factor *factor);

    // Variable
    VarIndex extractVariable(Variable *variable);
    void extractModifiesRelationship(Variable *variable);
    void extractUsesRelationship(Variable *variable);

    // ConstantValue
    ConstIndex extractConstantValue(ConstantValue *constantValue);
};