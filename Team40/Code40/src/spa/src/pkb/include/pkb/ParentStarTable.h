#pragma once

#include "Abstractions.h"

#include <map>
#include <set>

class ParentStarTable {

public:
    ParentStarTable();
    bool insertParentStar(StmtIndex stmt1, StmtIndex stmt2);
    set<StmtIndex> getParentStarStmts(StmtIndex stmt);
    set<StmtIndex> getChildStarStmts(StmtIndex stmt);
    bool parentStar(StmtIndex stmt1, StmtIndex stmt2);

private:
    //value is parent of key (value contains all parents of key)
    map<StmtIndex, set<StmtIndex>> parentStarMap;
    //value is child of key (value contains all children of key)
    map<StmtIndex, set<StmtIndex>> childStarMap;
    void insertIntoMaps(StmtIndex stmt1, StmtIndex stmt2);
};

