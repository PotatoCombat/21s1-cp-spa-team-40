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
    void extractLastExecutedStatement(vector<Statement *> statementList,
                                      vector<Statement *> &result);
    static void updateLastExecutedStatementsForCurrentProc(
        set<StmtIndex> lastExecutedStmtIndices);

public:
    // Constructor
    explicit DepthFirstExtractor(PKB *pkb);

    // Program
    void extract(Program *program);

    // Procedure
    void extractProcedure(Procedure *procedure);

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
};
