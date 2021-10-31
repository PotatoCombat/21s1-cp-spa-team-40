#pragma once

#include "common/model/Program.h"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/ExtractionContext.h"

class BreadthFirstExtractor {
private:
    vector<vector<Statement *>> statementLists;
    PKB *pkb;

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
