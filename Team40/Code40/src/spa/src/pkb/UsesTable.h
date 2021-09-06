#pragma once

#include <map>
#include <set>
#include <Abstractions.h>

class UsesTable {

public:
    UsesTable();
    bool insertStmtUsingVar(StatementIndex stmt, VarIndex var);
    bool insertProcUsingVar(StatementIndex stmt, VarIndex var);
    set<VarIndex> getVarsUsedByStmt(StatementIndex stmt);
    set<StatementIndex> getStmtsUsingVar(VarIndex var);
    set<VarIndex> getVarsUsedByProc(ProcedureIndex proc);
    set<ProcedureIndex> getProcsUsingVar(VarIndex var);
    bool stmtUses(StatementIndex stmt, VarIndex var);
    bool procUses(ProcedureIndex proc, VarIndex var);

private:
    map<StatementIndex, set<VarIndex>> stmtUsesVarsMap;
    map<VarIndex, set<StatementIndex>> varUsedByStmtsMap;
    map<ProcedureIndex, set<VarIndex>> procUsesVarsMap;
    map<VarIndex, set<ProcedureIndex>> varUsedByProcsMap;
};

UsesTable::UsesTable() = default;

bool UsesTable::insertStmtUsingVar(StatementIndex stmt, VarIndex var) {
    if (stmtUsesVarsMap.count(stmt) <= 0) {
        stmtUsesVarsMap.insert(pair<StatementIndex, set<VarIndex>>(stmt, {}));
    }
    stmtUsesVarsMap[stmt].insert(var);

    if (varUsedByStmtsMap.count(var) <= 0) {
        varUsedByStmtsMap.insert(pair<VarIndex , set<StatementIndex>>(var, {}));
    }
    varUsedByStmtsMap[var].insert(stmt);
    return true;
}

bool UsesTable::insertProcUsingVar(ProcedureIndex proc, VarIndex var) {
    if (procUsesVarsMap.count(proc) <= 0) {
        procUsesVarsMap.insert(pair<ProcedureIndex, set<VarIndex>>(proc, {}));
    }
    procUsesVarsMap[proc].insert(var);

    if (varUsedByProcsMap.count(var) <= 0) {
        varUsedByProcsMap.insert(pair<VarIndex , set<ProcedureIndex>>(var, {}));
    }
    varUsedByProcsMap[var].insert(proc);
    return true;
}

set<VarIndex> UsesTable::getVarsUsedByStmt(StatementIndex stmt) {
    return stmtUsesVarsMap.at(stmt);
}

set<StatementIndex> UsesTable::getStmtsUsingVar(VarIndex var) {
    return varUsedByStmtsMap.at(var);
}

set<VarIndex> UsesTable::getVarsUsedByProc(ProcedureIndex proc) {
    return procUsesVarsMap.at(stmt);
}

set<ProcedureIndex> UsesTable::getProcsUsingVar(VarIndex var) {
    return varUsedByProcsMap.at(var);
}

//Checks if given stmt uses given var
bool UsesTable::stmtUses(StatementIndex stmt, VarIndex var) {
    set<VarIndex> varIndexes = stmtUsesVarsMap[stmt];
    if (varIndexes.find(var) != varIndexes.end()) {
        return true;
    }
    return false;
}

//Checks if given proc uses given var
bool UsesTable::procUses(ProcedureIndex proc, VarIndex var) {
    set<VarIndex> varIndexes = procUsesVarsMap[proc];
    if (varIndexes.find(var) != varIndexes.end()) {
        return true;
    }
    return false;
}

