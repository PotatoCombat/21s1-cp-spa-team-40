#pragma once

#include "Abstractions.h"

#include <map>
#include <set>

class ModifiesTable {

public:
    ModifiesTable();
    bool insertStmtModifyingVar(StmtIndex stmt, VarIndex var);
    bool insertProcModifyingVar(ProcIndex proc, VarIndex var);
    set<VarIndex> getVarsModifiedByStmt(StmtIndex stmt);
    set<StmtIndex> getStmtsModifyingVar(VarIndex var);
    set<VarIndex> getVarsModifiedByProc(ProcIndex proc);
    set<ProcIndex> getProcsModifyingVar(VarIndex var);
    bool stmtModifies(StmtIndex stmt, VarIndex var);
    bool procModifies(ProcIndex proc, VarIndex var);

private:
    map<StmtIndex, set<VarIndex>> stmtModifiesVarsMap;
    map<VarIndex, set<StmtIndex>> varModifiedByStmtsMap;
    map<ProcIndex, set<VarIndex>> procModifiesVarsMap;
    map<VarIndex, set<ProcIndex>> varModifiedByProcsMap;
};
