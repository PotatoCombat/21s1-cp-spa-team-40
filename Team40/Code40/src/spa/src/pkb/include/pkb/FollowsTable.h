#pragma once

#include "Abstractions.h"
#include <map>

class FollowsTable {

public:
    FollowsTable();
    bool insertFollows(StmtIndex stmt1, StmtIndex stmt2);
    StmtIndex getFollowingStmt(StmtIndex stmt);
    StmtIndex getPrecedingStmt(StmtIndex stmt);
    bool follows(StmtIndex stmt1, StmtIndex stmt2);

private:
    //value follows key
    map<StmtIndex, StmtIndex> followsMap;
    //key follows value (value is followed by key)
    map<StmtIndex, StmtIndex> followedByMap;
};

