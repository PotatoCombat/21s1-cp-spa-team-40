#pragma once

#include <set>
#include <vector>

#include "pkb/Abstractions.h"
#include "pkb/Iterator.h"
#include "pkb/PKB.h"

#include "common/model/Statement.h"

using namespace std;

// based on the Example procedure in Test40/sample_source
class PKBStub : public PKB {
public:
    Iterator<StmtIndex> getAllStmts();

    Iterator<StmtIndex> getAllStmts(StatementType stmtType);

    StatementType getStmtType(StmtIndex stmt);

    StmtIndex getFollowingStmt(StmtIndex stmt);
    StmtIndex getPrecedingStmt(StmtIndex stmt);
    bool follows(StmtIndex stmt1, StmtIndex stmt2);

    set<StmtIndex> getFollowingStarStmts(StmtIndex stmt);
    set<StmtIndex> getPrecedingStarStmts(StmtIndex stmt);
    bool followsStar(StmtIndex stmt1, StmtIndex stmt2);

    StmtIndex getParentStmt(StmtIndex stmt);
    set<StmtIndex> getChildStmts(StmtIndex stmt);
    bool parent(StmtIndex stmt1, StmtIndex stmt2);

    set<StmtIndex> getParentStarStmts(StmtIndex stmt);
    set<StmtIndex> getChildStarStmts(StmtIndex stmt);
    bool parentStar(StmtIndex stmt1, StmtIndex stmt2);
};
