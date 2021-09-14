#pragma once

#include <vector>
#include <set>

#include "common/model/Abstractions.h"
#include "pkb/Iterator.h"
#include "pkb/PKB.h"

using namespace std;

// based on the q procedure in Test40/sample_source
class PKBStub2 : public PKB {
public:
    Iterator<StmtIndex> getAllStmts();

    StmtIndex getFollowingStmt(StmtIndex stmt);
    StmtIndex getPrecedingStmt(StmtIndex stmt);
    bool follows(StmtIndex stmt1, StmtIndex stmt2);

    StmtIndex getParentStmt(StmtIndex stmt);
    set<StmtIndex> getChildStmts(StmtIndex stmt);
    bool parent(StmtIndex stmt1, StmtIndex stmt2);
};
