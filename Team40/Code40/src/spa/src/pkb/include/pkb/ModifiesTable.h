#pragma once

#include "Abstractions.h"

#include <map>
#include <set>

class ModifiesTable {

public:
    ModifiesTable();

    /// Stores Modifies(proc, var).
    void insertProcModifyingVar(ProcIndex proc, VarIndex var);

    /// Stores Modifies(stmt, var).
    void insertStmtModifyingVar(StmtIndex stmt, VarIndex var);

    /// Selects p such that Modifies(p, var), where p is a Procedure.
    set<ProcIndex> getProcsModifyingVar(VarIndex var);

    /// Selects s such that Modifies(s, var), where s is a Statement.
    set<StmtIndex> getStmtsModifyingVar(VarIndex var);

    /// Selects v such that Modifies(proc, v), where v is a Variable.
    set<VarIndex> getVarsModifiedByProc(ProcIndex proc);

    /// Selects v such that Modifies(stmt, v), where v is a Variable.
    set<VarIndex> getVarsModifiedByStmt(StmtIndex stmt);

    /// Selects BOOLEAN such that Modifies(proc, var).
    bool procModifies(ProcIndex proc, VarIndex var);

    /// Selects BOOLEAN such that Modifies(stmt, var).
    bool stmtModifies(StmtIndex stmt, VarIndex var);

private:
    /// Stores p such that Modifies(p, v), where p is a Procedure.
    map<VarIndex, set<ProcIndex>> procsModifyingVarMap;

    /// Stores s such that Modifies(s, v), where s is a Statement.
    map<VarIndex, set<StmtIndex>> stmtsModifyingVarMap;

    /// Stores v such that Modifies(p, v), where v is a Variable.
    map<ProcIndex, set<VarIndex>> varsModifiedByProcMap;

    /// Stores v such that Modifies(s, v), where v is a Variable.
    map<StmtIndex, set<VarIndex>> varsModifiedByStmtMap;
};
