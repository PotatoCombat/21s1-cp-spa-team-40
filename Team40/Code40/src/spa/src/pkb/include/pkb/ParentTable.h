#pragma once

#include "Abstractions.h"

#include <map>
#include <set>

class ParentTable {

public:
    ParentTable();

    /// Stores Parent(stmt1, stmt2).
    void insertParent(StmtIndex stmt1, StmtIndex stmt2);

    /// Selects s such that Parent(s, stmt).
    StmtIndex getParentStmt(StmtIndex stmt);

    /// Selects s such that Parent(stmt, s).
    set<StmtIndex> getChildStmts(StmtIndex stmt);

    /// Selects BOOLEAN such that Follows(stmt1, stmt2).
    bool parent(StmtIndex stmt1, StmtIndex stmt2);

private:
    /// Stores s such that Parent(s, stmt).
    map<StmtIndex, StmtIndex> parentMap;

    /// Stores s such that Parent(stmt, s).
    map<StmtIndex, set<StmtIndex>> childMap;
};
