#pragma once

#include <map>
#include <Abstractions.h>

class ParentTable {

public:
    ParentTable();
    bool insertParent(StmtIndex stmt1, StmtIndex stmt2);
    StmtIndex getParentStmt(StmtIndex stmt);
    StmtIndex getChildStmt(StmtIndex stmt);
    bool parent(StmtIndex stmt1, StmtIndex stmt2);

private:
    //value is parent of key
    map<StmtIndex, StmtIndex> parentMap;
    //key is parent of value (value is child of key)
    map<StmtIndex, StmtIndex> childMap;
};
