#pragma once

#include "pkb/PKB.h"
#include "source_processor/design_extractor/ExtractionContext.h"
#include <common/model/Program.h>

class FollowsExtractor {
private:
    vector<vector<Statement>> statementLists;
    EntityContext<Statement> ctx;
    PKB *pkb;

public:
    explicit FollowsExtractor(PKB *pkb);
    // Program
    void extract(Program program);

    // Statement
    void extractStatement(Statement statement);
    void extractStatementList(vector<Statement> statementList);
};
