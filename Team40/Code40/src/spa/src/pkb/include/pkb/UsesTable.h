#pragma once

#include "Abstractions.h"

#include <map>
#include <set>

class UsesTable {

public:
    UsesTable();

    /// Stores Uses(proc, var).
    void insertProcUsingVar(ProcIndex proc, VarIndex var);

    /// Stores Uses(stmt, var).
    void insertStmtUsingVar(StmtIndex stmt, VarIndex var);

    /// Selects p such that Uses(p, var), where p is a Procedure.
    set<ProcIndex> getProcsUsingVar(VarIndex var);

    /// Selects s such that Uses(s, var), where s is a Statement.
    set<StmtIndex> getStmtsUsingVar(VarIndex var);

    /// Selects v such that Uses(proc, v), where v is a Variable.
    set<VarIndex> getVarsUsedByProc(ProcIndex proc);

    /// Selects v such that Uses(stmt, v), where v is a Variable.
    set<VarIndex> getVarsUsedByStmt(StmtIndex stmt);

    /// Selects BOOLEAN such that Uses(proc, var).
    bool procUses(ProcIndex proc, VarIndex var);

    /// Selects BOOLEAN such that Uses(stmt, var).
    bool stmtUses(StmtIndex stmt, VarIndex var);

private:
    /// Stores p such that Uses(p, v), where p is a Procedure.
    map<VarIndex, set<ProcIndex>> procsUsingVarMap;

    /// Stores s such that Uses(s, v), where s is a Statement.
    map<VarIndex, set<StmtIndex>> stmtsUsingVarMap;

    /// Stores v such that Uses(p, v), where v is a Variable.
    map<ProcIndex, set<VarIndex>> varsUsedByProcMap;

    /// Stores v such that Uses(s, v), where v is a Variable.
    map<StmtIndex, set<VarIndex>> varsUsedByStmtMap;
};
