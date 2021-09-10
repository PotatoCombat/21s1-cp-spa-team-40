#pragma once

#include "pkb/PKB.h"
#include "source_processor/ExtractionContext.h"
#include <common/model/Program.h>

class FollowsExtractor {
private:
    vector<vector<Statement *>> statementLists;
    EntityContext<Statement> ctx;
    PKB pkb;

public:
    FollowsExtractor(PKB pkb);
    // Program
    void handleProgram(Program program);

    // Statement
    void handleStatement(Statement *statement);
    void handleStatementList(const vector<Statement *> &statementList);
};
