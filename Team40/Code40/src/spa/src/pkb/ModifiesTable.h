#pragma once

#include <map>
#include <set>
#include <Abstractions.h>

class ModifiesTable {

public:
    ModifiesTable();
    bool insertStmtModifyingVar(StatementIndex stmt, VarIndex var);
    bool insertProcModifyingVar(StatementIndex stmt, VarIndex var);
    set<VarIndex> getVarsModifiedByStmt(StatementIndex stmt);
    set<StatementIndex> getStmtsModifyingVar(VarIndex var);
    set<VarIndex> getVarsModifiedByProc(ProcedureIndex proc);
    set<ProcedureIndex> getProcsModifyingVar(VarIndex var);
    bool stmtModifies(StatementIndex stmt, VarIndex var);
    bool procModifies(ProcedureIndex proc, VarIndex var);

private:
    map<StatementIndex, set<VarIndex>> stmtModifiesVarsMap;
    map<VarIndex, set<StatementIndex>> varModifiedByStmtsMap;
    map<ProcedureIndex, set<VarIndex>> procModifiesVarsMap;
    map<VarIndex, set<ProcedureIndex>> varModifiedByProcsMap;
};

ModifiesTable::ModifiesTable() = default;

bool ModifiesTable::insertStmtModifyingVar(StatementIndex stmt, VarIndex var) {
    if (stmtModifiesVarsMap.count(stmt) <= 0) {
        stmtModifiesVarsMap.insert(pair<StatementIndex, set<VarIndex>>(stmt, {}));
    }
    stmtModifiesVarsMap[stmt].insert(var);

    if (varModifiedByStmtsMap.count(var) <= 0) {
        varModifiedByStmtsMap.insert(pair<VarIndex , set<StatementIndex>>(var, {}));
    }
    varModifiedByStmtsMap[var].insert(stmt);
    return true;
}

bool ModifiesTable::insertProcModifyingVar(ProcedureIndex proc, VarIndex var) {
    if (procModifiesVarsMap.count(proc) <= 0) {
        procModifiesVarsMap.insert(pair<ProcedureIndex, set<VarIndex>>(proc, {}));
    }
    procModifiesVarsMap[proc].insert(var);

    if (varModifiedByProcsMap.count(var) <= 0) {
        varModifiedByProcsMap.insert(pair<VarIndex , set<ProcedureIndex>>(var, {}));
    }
    varModifiedByProcsMap[var].insert(proc);
    return true;
}

set<VarIndex> ModifiesTable::getVarsModifiedByStmt(StatementIndex stmt) {
    return stmtModifiesVarsMap.at(stmt);
}

set<StatementIndex> ModifiesTable::getStmtsModifyingVar(VarIndex var) {
    return varModifiedByStmtsMap.at(var);
}

set<VarIndex> ModifiesTable::getVarsModifiedByProc(ProcedureIndex proc) {
    return procModifiesVarsMap.at(stmt);
}

set<ProcedureIndex> ModifiesTable::getProcsModifyingVar(VarIndex var) {
    return varModifiedByProcsMap.at(var);
}

//Checks if given stmt uses given var
bool ModifiesTable::stmtModifies(StatementIndex stmt, VarIndex var) {
    set<VarIndex> varIndexes = stmtModifiesVarsMap[stmt];
    if (varIndexes.find(var) != varIndexes.end()) {
        return true;
    }
    return false;
}

//Checks if given proc uses given var
bool ModifiesTable::procModifies(ProcedureIndex proc, VarIndex var) {
    set<VarIndex> varIndexes = procModifiesVarsMap[proc];
    if (varIndexes.find(var) != varIndexes.end()) {
        return true;
    }
    return false;
}

