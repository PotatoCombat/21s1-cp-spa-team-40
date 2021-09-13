#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Procedure.h"
#include "common/model/Statement.h"
#include "common/model/Variable.h"

#include <map>
#include <set>

class UsesTable {

public:
    UsesTable();

    /// Stores Uses(proc, var).
    void insertProcUsingVar(Procedure *proc, Variable *var);

    /// Stores Uses(stmt, var).
    void insertStmtUsingVar(Statement *stmt, Variable *var);

    /// Selects p such that Uses(p, var), where p is a Procedure.
    set<ProcName> getProcsUsingVar(VarName var);

    /// Selects s such that Uses(s, var), where s is a Statement.
    set<StmtIndex> getStmtsUsingVar(VarName var);

    /// Selects v such that Uses(proc, v), where v is a Variable.
    set<VarName> getVarsUsedByProc(ProcName proc);

    /// Selects v such that Uses(stmt, v), where v is a Variable.
    set<VarName> getVarsUsedByStmt(StmtIndex stmt);

    /// Selects BOOLEAN such that Uses(proc, var).
    bool procUses(ProcName proc, VarName var);

    /// Selects BOOLEAN such that Uses(stmt, var).
    bool stmtUses(StmtIndex stmt, VarName var);

private:
    /// Stores p such that Uses(p, v), where p is a Procedure.
    map<VarName, set<ProcName>> procsUsingVarMap;

    /// Stores s such that Uses(s, v), where s is a Statement.
    map<VarName, set<StmtIndex>> stmtsUsingVarMap;

    /// Stores v such that Uses(p, v), where v is a Variable.
    map<ProcName, set<VarName>> varsUsedByProcMap;

    /// Stores v such that Uses(s, v), where v is a Variable.
    map<StmtIndex, set<VarName>> varsUsedByStmtMap;
};
