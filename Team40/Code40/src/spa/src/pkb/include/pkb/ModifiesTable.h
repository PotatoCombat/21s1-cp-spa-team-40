#pragma once

#include "Abstractions.h"
#include "common/model/Procedure.h"
#include "common/model/Statement.h"
#include "common/model/Variable.h"

#include <map>
#include <set>

class ModifiesTable {

public:
    ModifiesTable();

    /// Stores Modifies(proc, var).
    void insertProcModifyingVar(Procedure *proc, Variable *var);

    /// Stores Modifies(stmt, var).
    void insertStmtModifyingVar(Statement *stmt, Variable *var);

    /// Selects p such that Modifies(p, var), where p is a Procedure.
    set<ProcName> getProcsModifyingVar(VarName var);

    /// Selects s such that Modifies(s, var), where s is a Statement.
    set<StmtIndex> getStmtsModifyingVar(VarName var);

    /// Selects v such that Modifies(proc, v), where v is a Variable.
    set<VarName> getVarsModifiedByProc(ProcName proc);

    /// Selects v such that Modifies(stmt, v), where v is a Variable.
    set<VarName> getVarsModifiedByStmt(StmtIndex stmt);

    /// Selects BOOLEAN such that Modifies(proc, var).
    bool procModifies(ProcName proc, VarName var);

    /// Selects BOOLEAN such that Modifies(stmt, var).
    bool stmtModifies(StmtIndex stmt, VarName var);

private:
    /// Stores p such that Modifies(p, v), where p is a Procedure.
    map<VarName, set<ProcName>> procsModifyingVarMap;

    /// Stores s such that Modifies(s, v), where s is a Statement.
    map<VarName, set<StmtIndex>> stmtsModifyingVarMap;

    /// Stores v such that Modifies(p, v), where v is a Variable.
    map<ProcName, set<VarName>> varsModifiedByProcMap;

    /// Stores v such that Modifies(s, v), where v is a Variable.
    map<StmtIndex, set<VarName>> varsModifiedByStmtMap;
};
