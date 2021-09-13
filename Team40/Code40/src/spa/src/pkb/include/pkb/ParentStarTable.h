#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Statement.h"

#include <map>
#include <set>

class ParentStarTable {

public:
    ParentStarTable();

    /// Stores Parent*(stmt1, stmt2).
    void insertParentStar(Statement stmt1, Statement stmt2);

    /// Selects s such that Parent*(s, stmt).
    set<StmtIndex> getParentStarStmts(StmtIndex stmt);

    /// Selects s such that Parent*(stmt, s).
    set<StmtIndex> getChildStarStmts(StmtIndex stmt);

    /// Selects BOOLEAN such that Parent*(stmt1, stmt2).
    bool parentStar(StmtIndex stmt1, StmtIndex stmt2);

private:
    /// Stores s such that Parent*(s, stmt).
    map<StmtIndex, set<StmtIndex>> parentStarMap;

    /// Stores s such that Parent*(stmt, s).
    map<StmtIndex, set<StmtIndex>> childStarMap;

    void insertIntoMaps(StmtIndex stmt1, StmtIndex stmt2);
};
