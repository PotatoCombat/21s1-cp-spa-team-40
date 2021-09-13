#include "pkb/ModifiesTable.h"

ModifiesTable::ModifiesTable() = default;

void ModifiesTable::insertProcModifyingVar(Procedure proc, Variable var) {
    VarName varName = var.getName();
    ProcName procName = proc.getName();
    auto search1 = procsModifyingVarMap.find(varName);
    if (search1 == procsModifyingVarMap.end()) {
        procsModifyingVarMap[varName] = { procName };
    }
    else {
        search1->second.insert(procName);
    }

    auto search2 = varsModifiedByProcMap.find(procName);
    if (search2 == varsModifiedByProcMap.end()) {
        varsModifiedByProcMap[procName] = { varName };
    }
    else {
        search2->second.insert(varName);
    }
}

void ModifiesTable::insertStmtModifyingVar(Statement stmt, Variable var) {
    StmtIndex stmtIndex = stmt.getIndex();
    VarName varName = var.getName();
    auto search1 = stmtsModifyingVarMap.find(varName);
    if (search1 == stmtsModifyingVarMap.end()) {
        stmtsModifyingVarMap[varName] = { stmtIndex };
    }
    else {
        search1->second.insert(stmtIndex);
    }

    auto search2 = varsModifiedByStmtMap.find(stmtIndex);
    if (search2 == varsModifiedByStmtMap.end()) {
        varsModifiedByStmtMap[stmtIndex] = { varName };
    }
    else {
        search2->second.insert(varName);
    }
}

set<ProcName> ModifiesTable::getProcsModifyingVar(VarName var) {
    auto result = procsModifyingVarMap.find(var);
    if (result == procsModifyingVarMap.end()) {
        return {};
    }
    return result->second;
}

set<StmtIndex> ModifiesTable::getStmtsModifyingVar(VarName var) {
    auto result = stmtsModifyingVarMap.find(var);
    if (result == stmtsModifyingVarMap.end()) {
        return {};
    }
    return result->second;
}

set<VarName> ModifiesTable::getVarsModifiedByProc(ProcName proc) {
    auto result = varsModifiedByProcMap.find(proc);
    if (result == varsModifiedByProcMap.end()) {
        return {};
    }
    return result->second;
}

set<VarName> ModifiesTable::getVarsModifiedByStmt(StmtIndex stmt) {
    auto result = varsModifiedByStmtMap.find(stmt);
    if (result == varsModifiedByStmtMap.end()) {
        return {};
    }
    return result->second;
}

bool ModifiesTable::procModifies(ProcName proc, VarName var) {
    auto result = varsModifiedByProcMap.find(proc);
    if (result == varsModifiedByProcMap.end()) {
        return false;
    }
    auto vars = result->second;
    return vars.find(var) != vars.end();
}

bool ModifiesTable::stmtModifies(StmtIndex stmt, VarName var) {
    auto result = varsModifiedByStmtMap.find(stmt);
    if (result == varsModifiedByStmtMap.end()) {
        return false;
    }
    auto vars = result->second;
    return vars.find(var) != vars.end();
}
