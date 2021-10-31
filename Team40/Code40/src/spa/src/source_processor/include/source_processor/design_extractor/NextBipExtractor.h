#pragma once

#include "common/model/Program.h"
#include "pkb/PKB.h"
#include "source_processor/design_extractor/ExtractionContext.h"

class NextBipExtractor {
private:
    PKB *pkb;
    vector<set<ProgLineIndex>> callbacks;
    set<ProgLineIndex> visited;

public:
    explicit NextBipExtractor(PKB *pkb);

    void extract(Program *program);

    void extractProcedure(Procedure *procedure);

    void extractStatement(Statement *statement);
    void extractCallStatement(Statement *callStatement);
    void extractNonCallStatement(Statement *statement);
};