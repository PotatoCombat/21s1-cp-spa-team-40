#pragma once

#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/condition/NotCondition.h"
#include "common/model/condition/SingleCondition.h"
#include "common/model/statement/AssignStatement.h"
#include "common/model/statement/Statement.h"
#include "common/model/term/Term.h"
#include "pkb/PKB.h"
#include "source_processor/ExtractionContext.h"

class DesignExtractor {
    PKB pkb;

public:
    DesignExtractor(PKB pkb);

    // Program
    void handleProgram(Program program);

    // Procedure
    ProcIndex handleProcedure(Procedure *program);

    // Statement
    StmtIndex handleStatement(Statement *statement);
    StmtIndex handleAssignStatement(Statement *assignStatement);
    StmtIndex handleCallStatement(Statement *callStatement);
    StmtIndex handleIfStatement(Statement *ifStatement);
    void handleContextualRelationships(StmtIndex stmtIndex);

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

    // ConstantValue
    ConstIndex handleConstantValue(ConstantValue *constantValue);
};