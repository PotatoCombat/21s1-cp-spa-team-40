#pragma once

#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/condition/NotCondition.h"
#include "common/model/condition/SingleCondition.h"
#include "common/model/expression/SingleTermExpression.h"
#include "common/model/statement/AssignStatement.h"
#include "common/model/statement/CallStatement.h"
#include "common/model/statement/Statement.h"
#include "common/model/term/Term.h"
#include "pkb/PKB.h"
#include "source_processor/ExtractionContext.h"
#include "source_processor/FollowsExtractor.h"
#include "source_processor/ParentsExtractor.h"

class DesignExtractor {
    PKB pkb;

public:
    explicit DesignExtractor(PKB pkb);

    // Program
    void extractEntities(Program program);
    void extractRelationships(const Program &program);
    void extractFollowsRelationship(Program program);
    void extractParentsRelationship(Program program);

    // Procedure
    ProcIndex handleProcedure(Procedure *program);

    // Statement
    void handleStatementList(vector<Statement *> statement);
    StmtIndex handleStatement(Statement *statement);
    StmtIndex handleAssignStatement(Statement *assignStatement);
    StmtIndex handleCallStatement(CallStatement *callStatement);
    StmtIndex handleIfStatement(Statement *ifStatement);
    void handleParentRelationship(StmtIndex stmtIndex);

    // Condition
    void handleCondition(Condition *condition);
    void handleSingleCondition(SingleCondition *singleCondition);
    void handleNotCondition(NotCondition *notCondition);
    void handleBinaryCondition(Condition *binaryCondition);

    // Relation
    void handleRelation(Relation *relation);

    // Expression
    void handleExpression(Expression *expression);
    void handleSingleTermExpression(Expression *singleTermExpression);

    // Term
    void handleTerm(Term *term);
    void handleSingleFactorTerm(Term *singleFactorTerm);
    void handleMultiFactorTerm(Term *multiFactorTerm);

    // Factor
    void handleFactor(Factor *factor);

    // Variable
    VarIndex handleVariable(Variable *variable);
    void handleModifiesRelationship(Variable *variable);
    void handleUsesRelationship(Variable *variable);

    // ConstantValue
    ConstIndex handleConstantValue(ConstantValue *constantValue);
};