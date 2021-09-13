#include "pkb/UsesTable.h"

UsesTable::UsesTable() = default;

void UsesTable::insertProcUsingVar(Procedure proc, Variable var) {
    ProcName procName = proc.getName();
    VarName varName = var.getName();
    auto search1 = procsUsingVarMap.find(varName);
    if (search1 == procsUsingVarMap.end()) {
        procsUsingVarMap[varName] = { procName };
    }
    else {
        search1->second.insert(procName);
    }

    auto search2 = varsUsedByProcMap.find(procName);
    if (search2 == varsUsedByProcMap.end()) {
        varsUsedByProcMap[procName] = { varName };
    }
    else {
        search2->second.insert(varName);
    }
}

void UsesTable::insertStmtUsingVar(Statement stmt, Variable var) {
    StmtIndex stmtIndex = stmt.getIndex();
    VarName varName = var.getName();
    auto search1 = stmtsUsingVarMap.find(varName);
    if (search1 == stmtsUsingVarMap.end()) {
        stmtsUsingVarMap[varName] = { stmtIndex };
    }
    else {
        search1->second.insert(stmtIndex);
    }

    auto search2 = varsUsedByStmtMap.find(stmtIndex);
    if (search2 == varsUsedByStmtMap.end()) {
        varsUsedByStmtMap[stmtIndex] = { varName };
    }
    else {
        search2->second.insert(varName);
    }
}

set<ProcName> UsesTable::getProcsUsingVar(VarName var) {
    auto result = procsUsingVarMap.find(var);
    if (result == procsUsingVarMap.end()) {
        return {};
    }
    return result->second;
}

set<StmtIndex> UsesTable::getStmtsUsingVar(VarName var) {
    auto result = stmtsUsingVarMap.find(var);
    if (result == stmtsUsingVarMap.end()) {
        return {};
    }
    return result->second;
}

set<VarName> UsesTable::getVarsUsedByProc(ProcName proc) {
    auto result = varsUsedByProcMap.find(proc);
    if (result == varsUsedByProcMap.end()) {
        return {};
    }
    return result->second;
}

set<VarName> UsesTable::getVarsUsedByStmt(StmtIndex stmt) {
    auto result = varsUsedByStmtMap.find(stmt);
    if (result == varsUsedByStmtMap.end()) {
        return {};
    }
    return result->second;
}

bool UsesTable::procUses(ProcName proc, VarName var) {
    auto result = varsUsedByProcMap.find(proc);
    if (result == varsUsedByProcMap.end()) {
        return false;
    }
    auto vars = result->second;
    return vars.find(var) != vars.end();
}

bool UsesTable::stmtUses(StmtIndex stmt, VarName var) {
    auto result = varsUsedByStmtMap.find(stmt);
    if (result == varsUsedByStmtMap.end()) {
        return false;
    }
    auto vars = result->second;
    return vars.find(var) != vars.end();
}
