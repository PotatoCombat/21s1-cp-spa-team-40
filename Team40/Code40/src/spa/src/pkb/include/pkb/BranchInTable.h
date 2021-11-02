#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Procedure.h"
#include <map>

class BranchInTable {

public:
    BranchInTable();

    /// Stores relation: stmt1 branches into stmt2
    void insertBranchIn(Statement *stmt1, Statement *stmt2);

    /// Selects s such that stmt branches into s
    StmtIndex getBranchInToStmt(StmtIndex stmt);

    /// Selects s such that s branches into stmt
    set<StmtIndex> getBranchInFromStmts(StmtIndex stmt);

    /// Selects BOOLEAN such that stmt1 branches into stmt2
    bool branchIn(StmtIndex stmt1, StmtIndex stmt2);

private:
    /// Stores <s1, s2> such that s1 branches in to s2
    map<StmtIndex, StmtIndex> branchInToMap;

    /// Stores <s1, s2> such that s1 branches in from s2
    map<StmtIndex, set<StmtIndex>> branchInFromMap;
};
