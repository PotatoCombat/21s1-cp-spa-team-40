#pragma once

#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Statement.h"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/ExtractionContext.h"
#include <set>

class DepthFirstExtractor {
private:
    PKB *pkb;

    // Utility
    void extractLastLeafStatements(vector<Statement *> statementList,
                                   vector<Statement *> &result);

public:
    // Constructor
    explicit DepthFirstExtractor(PKB *pkb);

    // Program
    void extract(Program *program);

    // Procedure
    void extractProcedure(Procedure *program);

    // Statement
    void extractStatement(Statement *statement);
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

    // ConstantValue
    void extractConstantValue(ConstantValue *constantValue);

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
};
