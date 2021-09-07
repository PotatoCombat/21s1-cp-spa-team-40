#include "ParentStarTable.h"

ParentStarTable::ParentStarTable() = default;

//NOTE: stmt2 is parent of stmt1
void ParentStarTable::insertIntoMaps(StmtIndex stmt1, StmtIndex stmt2) {
  if (parentStarMap.count(stmt1) == 0) {
    parentStarMap.insert(pair<StmtIndex, set<StmtIndex>>(stmt1, {}));
  }
  parentStarMap[stmt1].insert(stmt2);

  if (childStarMap.count(stmt2) == 0) {
    childStarMap.insert(pair<StmtIndex, set<StmtIndex>>(stmt2, {}));
  }
  childStarMap[stmt2].insert(stmt1);
}

//NOTE: stmt2 is parent of stmt1
bool ParentStarTable::insertParentStar(StmtIndex stmt1, StmtIndex stmt2) {
  insertIntoMaps(stmt1, stmt2);

  //Adding parent*(prevStmts, stmt2)
  if (childStarMap.count(stmt1) > 0) {
    for (const StmtIndex& prevStmt : childStarMap[stmt1]) {
      insertIntoMaps(prevStmt, stmt2);
    }
  }

  //Adding parent*(stmt1, nextStmts)
  if (parentStarMap.count(stmt2) > 0) {
    for (const StmtIndex& nextStmt : parentStarMap[stmt2]) {
      insertIntoMaps(stmt1, nextStmt);
    }
  }

  return true;
}

set<StmtIndex> ParentStarTable::getParentStarStmts(StmtIndex stmt) {
  return parentStarMap[stmt];
}

set<StmtIndex> ParentStarTable::getChildStarStmts(StmtIndex stmt) {
  return childStarMap[stmt];
}

//Parent*(stmt1, stmt2)
bool ParentStarTable::parentStar(StmtIndex stmt1, StmtIndex stmt2) {
  set<StmtIndex> parentIndexes = parentStarMap[stmt1];
  return parentIndexes.find(stmt2) != parentIndexes.end();
}


