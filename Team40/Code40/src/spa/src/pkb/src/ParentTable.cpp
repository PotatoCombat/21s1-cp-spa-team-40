#include "ParentTable.h"

ParentTable::ParentTable() = default;

//NOTE: stmt2 is parent of stmt1
bool ParentTable::insertParent(StmtIndex stmt1, StmtIndex stmt2) {
  parentMap[stmt1] = stmt2;
  childMap[stmt2] = stmt1;
  return true;
}

StmtIndex ParentTable::getParentStmt(StmtIndex stmt) {
  return parentMap.at(stmt);
}

StmtIndex ParentTable::getChildStmt(StmtIndex stmt) {
  return childMap.at(stmt);
}

//Parent(stmt1, stmt2)
bool ParentTable::parent(StmtIndex stmt1, StmtIndex stmt2) {
  StmtIndex parentIndex = parentMap.at(stmt1);
  return parentIndex == stmt2;
}

