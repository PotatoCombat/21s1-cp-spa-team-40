#pragma once

#include "common/model/Program.h"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/ExtractionContext.h"

class BreadthFirstExtractor {
private:
    vector<vector<Statement *>> statementLists;
    PKB *pkb;

    void extractTransitiveUsesRelationship(Statement *callStatement,
                                           Procedure *currentProcedure,
                                           ProcName calleeName);
    void extractTransitiveModifiesRelationship(Statement *callStatement,
                                               Procedure *currentProcedure,
                                               ProcName calleeName);
    void updateLastExecutableStatements(Statement *callStatement,
                                        Procedure *currentProcedure,
                                        ProcName calleeName);

public:
    explicit BreadthFirstExtractor(PKB *pkb);
    // Program
    void extract(Program *program);

    // Procedure
    void extractProcedure(Procedure *procedure);

    // Statement
    void extractStatement(Statement *statement);
    void extractIfStatement(Statement *ifStatement);
    void extractWhileStatement(Statement *whileStatement);
    void extractCallStatement(Statement *callStatement);
};
