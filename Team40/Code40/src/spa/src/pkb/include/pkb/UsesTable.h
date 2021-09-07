#pragma once

#include "Abstractions.h"

#include <map>
#include <set>

class UsesTable {

public:
    UsesTable();
    bool insertStmtUsingVar(StmtIndex stmt, VarIndex var);
    bool insertProcUsingVar(ProcIndex proc, VarIndex var);
    set<VarIndex> getVarsUsedByStmt(StmtIndex stmt);
    set<StmtIndex> getStmtsUsingVar(VarIndex var);
    set<VarIndex> getVarsUsedByProc(ProcIndex proc);
    set<ProcIndex> getProcsUsingVar(VarIndex var);
    bool stmtUses(StmtIndex stmt, VarIndex var);
    bool procUses(ProcIndex proc, VarIndex var);

private:
    map<StmtIndex, set<VarIndex>> stmtUsesVarsMap;
    map<VarIndex, set<StmtIndex>> varUsedByStmtsMap;
    map<ProcIndex, set<VarIndex>> procUsesVarsMap;
    map<VarIndex, set<ProcIndex>> varUsedByProcsMap;
};
