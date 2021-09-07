#pragma once

#include "Abstractions.h"

#include <map>
#include <set>

class FollowsStarTable {

public:
    FollowsStarTable();
    bool insertFollowsStar(StmtIndex stmt1, StmtIndex stmt2);
    set<StmtIndex> getFollowingStarStmts(StmtIndex stmt);
    set<StmtIndex> getPrecedingStarStmts(StmtIndex stmt);
    bool followsStar(StmtIndex stmt1, StmtIndex stmt2);

private:
    //stmt2 follows stmt1 (i.e. key comes before stmts in value)
    map<StmtIndex, set<StmtIndex>> followsStarMap;
    //stmt2 is followed by stmt1 (i.e. stmts in value comes before key)
    map<StmtIndex, set<StmtIndex>> followedByStarMap;
    void insertIntoMaps(StmtIndex stmt1, StmtIndex stmt2);
};
