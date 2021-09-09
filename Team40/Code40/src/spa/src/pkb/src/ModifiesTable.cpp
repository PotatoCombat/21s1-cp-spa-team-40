#include "pkb/ModifiesTable.h"

ModifiesTable::ModifiesTable() = default;

void ModifiesTable::insertProcModifyingVar(ProcIndex proc, VarIndex var) {
    auto search1 = procsModifyingVarMap.find(var);
    if (search1 == procsModifyingVarMap.end()) {
        procsModifyingVarMap[var] = { proc };
    }
    else {
        search1->second.insert(proc);
    }

    auto search2 = varsModifiedByProcMap.find(proc);
    if (search2 == varsModifiedByProcMap.end()) {
        varsModifiedByProcMap[proc] = { var };
    }
    else {
        search2->second.insert(var);
    }
}

void ModifiesTable::insertStmtModifyingVar(StmtIndex stmt, VarIndex var) {
    auto search1 = stmtsModifyingVarMap.find(var);
    if (search1 == stmtsModifyingVarMap.end()) {
        stmtsModifyingVarMap[var] = { stmt };
    }
    else {
        search1->second.insert(stmt);
    }

    auto search2 = varsModifiedByStmtMap.find(stmt);
    if (search2 == varsModifiedByStmtMap.end()) {
        varsModifiedByStmtMap[stmt] = { var };
    }
    else {
        search2->second.insert(var);
    }
}

set<ProcIndex> ModifiesTable::getProcsModifyingVar(VarIndex var) {
    auto result = procsModifyingVarMap.find(var);
    if (result == procsModifyingVarMap.end()) {
        return {};
    }
    return result->second;
}

set<StmtIndex> ModifiesTable::getStmtsModifyingVar(VarIndex var) {
    auto result = stmtsModifyingVarMap.find(var);
    if (result == stmtsModifyingVarMap.end()) {
        return {};
    }
    return result->second;
}

set<VarIndex> ModifiesTable::getVarsModifiedByProc(ProcIndex proc) {
    auto result = varsModifiedByProcMap.find(proc);
    if (result == varsModifiedByProcMap.end()) {
        return {};
    }
    return result->second;
}

set<VarIndex> ModifiesTable::getVarsModifiedByStmt(StmtIndex stmt) {
    auto result = varsModifiedByStmtMap.find(stmt);
    if (result == varsModifiedByStmtMap.end()) {
        return {};
    }
    return result->second;
}

bool ModifiesTable::procModifies(ProcIndex proc, VarIndex var) {
    auto result = varsModifiedByProcMap.find(proc);
    if (result == varsModifiedByProcMap.end()) {
        return false;
    }
    auto vars = result->second;
    return vars.find(var) != vars.end();
}

bool ModifiesTable::stmtModifies(StmtIndex stmt, VarIndex var) {
    auto result = varsModifiedByStmtMap.find(stmt);
    if (result == varsModifiedByStmtMap.end()) {
        return false;
    }
    auto vars = result->second;
    return vars.find(var) != vars.end();
}
