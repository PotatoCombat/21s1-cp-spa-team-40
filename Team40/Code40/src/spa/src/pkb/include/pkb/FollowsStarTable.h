#pragma once

#include "Abstractions.h"
#include "common/model/Statement.h"

#include <map>
#include <set>

class FollowsStarTable {

public:
    FollowsStarTable();

    /// Stores Follows*(stmt1, stmt2).
    void insertFollowsStar(Statement *stmt1, Statement *stmt2);

    /// Selects s such that Follows*(stmt, s).
    set<StmtIndex> getFollowingStarStmts(StmtIndex stmt);

    /// Selects s such that Follows*(s, stmt).
    set<StmtIndex> getPrecedingStarStmts(StmtIndex stmt);

    /// Selects BOOLEAN such that Follows(stmt1, stmt2).
    bool followsStar(StmtIndex stmt1, StmtIndex stmt2);

private:
    /// Stores s such that Follows*(stmt, s).
    map<StmtIndex, set<StmtIndex>> followsStarMap;

    /// Stores s such that Follows*(s, stmt).
    map<StmtIndex, set<StmtIndex>> followedByStarMap;

    void insertIntoMaps(StmtIndex stmt1, StmtIndex stmt2);
};
