#pragma once

#include <set>

#include "common/model/Procedure.h"
#include "common/model/Program.h"
#include "common/model/Statement.h"

#include "pkb/PKB.h"

#include "source_processor/design_extractor/ExtractionContext.h"

class DepthFirstExtractor {
private:
    PKB *pkb;

    // Procedure
    void extractProcedure(Procedure *procedure);

    // Statement
    void extractStatement(Statement *statement);
    void extractAssignStatement(Statement *assignStatement);
    void extractAssignStatementLHS(Statement *assignStatement);
    void extractAssignStatementRHS(Statement *assignStatement);
    void extractCallStatement(Statement *callStatement);
    void extractProcDependency(Statement *callStatement);
    void extractIfStatement(Statement *ifStatement);
    void extractIfCondition(Statement *ifStatement);
    void extractIfThenStatements(Statement *ifStatement);
    void extractIfElseStatements(Statement *ifStatement);
    void extractIfStatementCleanup(Statement *ifStatement);
    void extractPrintStatement(Statement *printStatement);
    void extractReadStatement(Statement *readStatement);
    void extractWhileStatement(Statement *whileStatement);
    void extractWhileCondition(Statement *whileStatement);
    void extractWhileThenStatements(Statement *whileStatement);
    void extractWhileStatementCleanup(Statement *whileStatement);

    // Variable
    void extractVariable(Variable *variable);
    void extractModifiesRelationship(Variable *variable);
    void extractUsesRelationship(Variable *variable);

    // ConstantValue
    void extractConstantValue(ConstantValue *constantValue);

    // Utility
    void extractLastExecutableStatement(vector<Statement *> statementList,
                                        vector<Statement *> &result);
    static void updateLastExecutableStatementsForCurrentProc(
        set<StmtIndex> lastExecutableStmtIndices);

    // Relationships
    void extractParentRelationship(Statement *statement);
    void extractNextRelationship(Statement *statement);

public:
    // Constructor
    explicit DepthFirstExtractor(PKB *pkb);

    // Program
    void extract(Program *program);
};
