#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Procedure.h"
#include <map>

class BranchBackTable {

public:
    BranchBackTable();

    /// Stores relation: stmt1 branches back to stmt2
    void insertBranchBack(Statement *stmt1, Statement *stmt2);

    /// Selects s such that stmt branches back to s
    set<StmtIndex> getBranchBackToStmts(StmtIndex stmt);

    /// Selects s such that stmt branches back from s
    set<StmtIndex> getBranchBackFromStmts(StmtIndex stmt);

    /// Selects BOOLEAN such that stmt1 branches back to stmt2
    bool branchBack(StmtIndex stmt1, StmtIndex stmt2);

private:
    /// Stores <s1, s2> such that s1 branches back to s2
    map<StmtIndex, set<StmtIndex>> branchBackToMap;

    /// Stores <s1, s2> such that s1 branches back from s2
    map<StmtIndex, set<StmtIndex>> branchBackFromMap;
};
