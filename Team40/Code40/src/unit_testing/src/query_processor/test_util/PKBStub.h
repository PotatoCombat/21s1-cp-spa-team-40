#pragma once

#include <vector>

#include "Abstractions.h"
#include "Iterator.h"
#include "PKB.h"

using namespace std;

// based on the Example procedure in Test40/sample_source
class PKBStub : public PKB {
public:
    Iterator<StmtIndex> getAllStmts();

    StmtIndex getFollowingStmt(StmtIndex stmt);
    StmtIndex getPrecedingStmt(StmtIndex stmt);
    bool follows(StmtIndex stmt1, StmtIndex stmt2);
};