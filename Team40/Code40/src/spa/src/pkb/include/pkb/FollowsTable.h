#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Statement.h"
#include <map>

class FollowsTable {

public:
    FollowsTable();

    /// Stores Follows(stmt1, stmt2).
    void insertFollows(Statement stmt1, Statement stmt2);

    /// Selects s such that Follows(stmt, s).
    StmtIndex getFollowingStmt(StmtIndex stmt);

    /// Selects s such that Follows(s, stmt).
    StmtIndex getPrecedingStmt(StmtIndex stmt);

    /// Selects BOOLEAN such that Follows(stmt1, stmt2).
    bool follows(StmtIndex stmt1, StmtIndex stmt2);

private:
    /// Stores s such that Follows(stmt, s).
    map<StmtIndex, StmtIndex> followsMap;

    /// Stores s such that Follows(s, stmt).
    map<StmtIndex, StmtIndex> followedByMap;
};
