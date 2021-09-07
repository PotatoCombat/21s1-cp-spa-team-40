#include <ModifiesTable.h>


ModifiesTable::ModifiesTable() = default;

bool ModifiesTable::insertStmtModifyingVar(StmtIndex stmt, VarIndex var) {
  if (stmtModifiesVarsMap.count(stmt) == 0) {
    stmtModifiesVarsMap.insert(pair<StmtIndex, set<VarIndex>>(stmt, {}));
  }
  stmtModifiesVarsMap[stmt].insert(var);

  if (varModifiedByStmtsMap.count(var) == 0) {
    varModifiedByStmtsMap.insert(pair<VarIndex , set<StmtIndex>>(var, {}));
  }
  varModifiedByStmtsMap[var].insert(stmt);
  return true;
}

bool ModifiesTable::insertProcModifyingVar(ProcIndex proc, VarIndex var) {
  if (procModifiesVarsMap.count(proc) == 0) {
    procModifiesVarsMap.insert(pair<ProcIndex, set<VarIndex>>(proc, {}));
  }
  procModifiesVarsMap[proc].insert(var);

  if (varModifiedByProcsMap.count(var) == 0) {
    varModifiedByProcsMap.insert(pair<VarIndex , set<ProcIndex>>(var, {}));
  }
  varModifiedByProcsMap[var].insert(proc);
  return true;
}

set<VarIndex> ModifiesTable::getVarsModifiedByStmt(StmtIndex stmt) {
  return stmtModifiesVarsMap.at(stmt);
}

set<StmtIndex> ModifiesTable::getStmtsModifyingVar(VarIndex var) {
  return varModifiedByStmtsMap.at(var);
}

set<VarIndex> ModifiesTable::getVarsModifiedByProc(ProcIndex proc) {
  return procModifiesVarsMap.at(proc);
}

set<ProcIndex> ModifiesTable::getProcsModifyingVar(VarIndex var) {
  return varModifiedByProcsMap.at(var);
}

//Checks if given stmt uses given var
bool ModifiesTable::stmtModifies(StmtIndex stmt, VarIndex var) {
  set<VarIndex> varIndexes = stmtModifiesVarsMap[stmt];
  return varIndexes.find(var) != varIndexes.end();
}

//Checks if given proc uses given var
bool ModifiesTable::procModifies(ProcIndex proc, VarIndex var) {
  set<VarIndex> varIndexes = procModifiesVarsMap[proc];
  return varIndexes.find(var) != varIndexes.end();
}


