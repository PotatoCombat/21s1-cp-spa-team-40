#include <UsesTable.h>


UsesTable::UsesTable() = default;

bool UsesTable::insertStmtUsingVar(StmtIndex stmt, VarIndex var) {
  if (stmtUsesVarsMap.count(stmt) == 0) {
    stmtUsesVarsMap.insert(pair<StmtIndex, set<VarIndex>>(stmt, {}));
  }
  stmtUsesVarsMap[stmt].insert(var);

  if (varUsedByStmtsMap.count(var) == 0) {
    varUsedByStmtsMap.insert(pair<VarIndex , set<StmtIndex>>(var, {}));
  }
  varUsedByStmtsMap[var].insert(stmt);
  return true;
}

bool UsesTable::insertProcUsingVar(ProcIndex proc, VarIndex var) {
  if (procUsesVarsMap.count(proc) == 0) {
    procUsesVarsMap.insert(pair<ProcIndex, set<VarIndex>>(proc, {}));
  }
  procUsesVarsMap[proc].insert(var);

  if (varUsedByProcsMap.count(var) == 0) {
    varUsedByProcsMap.insert(pair<VarIndex , set<ProcIndex>>(var, {}));
  }
  varUsedByProcsMap[var].insert(proc);
  return true;
}

set<VarIndex> UsesTable::getVarsUsedByStmt(StmtIndex stmt) {
  return stmtUsesVarsMap.at(stmt);
}

set<StmtIndex> UsesTable::getStmtsUsingVar(VarIndex var) {
  return varUsedByStmtsMap.at(var);
}

set<VarIndex> UsesTable::getVarsUsedByProc(ProcIndex proc) {
  return procUsesVarsMap.at(proc);
}

set<ProcIndex> UsesTable::getProcsUsingVar(VarIndex var) {
  return varUsedByProcsMap.at(var);
}

//Checks if given stmt uses given var
bool UsesTable::stmtUses(StmtIndex stmt, VarIndex var) {
  set<VarIndex> varIndexes = stmtUsesVarsMap[stmt];
  return varIndexes.find(var) != varIndexes.end();
}

//Checks if given proc uses given var
bool UsesTable::procUses(ProcIndex proc, VarIndex var) {
  set<VarIndex> varIndexes = procUsesVarsMap[proc];
  return varIndexes.find(var) != varIndexes.end();
}

