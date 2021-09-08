#include "pkb/UsesTable.h"

UsesTable::UsesTable() = default;

void UsesTable::insertProcUsingVar(ProcIndex proc, VarIndex var) {
    auto search1 = procsUsingVarMap.find(var);
    if (search1 == procsUsingVarMap.end()) {
        procsUsingVarMap[var] = { proc };
    }
    else {
        search1->second.insert(proc);
    }

    auto search2 = varsUsedByProcMap.find(proc);
    if (search2 == varsUsedByProcMap.end()) {
        varsUsedByProcMap[proc] = { var };
    }
    else {
        search2->second.insert(var);
    }
}

void UsesTable::insertStmtUsingVar(StmtIndex stmt, VarIndex var) {
    auto search1 = stmtsUsingVarMap.find(var);
    if (search1 == stmtsUsingVarMap.end()) {
        stmtsUsingVarMap[var] = { stmt };
    }
    else {
        search1->second.insert(stmt);
    }

    auto search2 = varsUsedByStmtMap.find(stmt);
    if (search2 == varsUsedByStmtMap.end()) {
        varsUsedByStmtMap[stmt] = { var };
    }
    else {
        search2->second.insert(var);
    }
}

set<ProcIndex> UsesTable::getProcsUsingVar(VarIndex var) {
    auto result = procsUsingVarMap.find(var);
    if (result == procsUsingVarMap.end()) {
        return {};
    }
    return result->second;
}

set<StmtIndex> UsesTable::getStmtsUsingVar(VarIndex var) {
    auto result = stmtsUsingVarMap.find(var);
    if (result == stmtsUsingVarMap.end()) {
        return {};
    }
    return result->second;
}

set<VarIndex> UsesTable::getVarsUsedByProc(ProcIndex proc) {
    auto result = varsUsedByProcMap.find(proc);
    if (result == varsUsedByProcMap.end()) {
        return {};
    }
    return result->second;
}

set<VarIndex> UsesTable::getVarsUsedByStmt(StmtIndex stmt) {
    auto result = varsUsedByStmtMap.find(stmt);
    if (result == varsUsedByStmtMap.end()) {
        return {};
    }
    return result->second;
}

bool UsesTable::procUses(ProcIndex proc, VarIndex var) {
    auto result = varsUsedByProcMap.find(proc);
    if (result == varsUsedByProcMap.end()) {
        return false;
    }
    auto vars = result->second;
    return vars.find(var) != vars.end();
}

bool UsesTable::stmtUses(StmtIndex stmt, VarIndex var) {
    auto result = varsUsedByStmtMap.find(stmt);
    if (result == varsUsedByStmtMap.end()) {
        return false;
    }
    auto vars = result->second;
    return vars.find(var) != vars.end();
}
