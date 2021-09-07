#include <FollowsStarTable.h>


FollowsStarTable::FollowsStarTable() = default;

void FollowsStarTable::insertIntoMaps(StmtIndex stmt1, StmtIndex stmt2) {
  if (followsStarMap.count(stmt1) == 0) {
    followsStarMap.insert(pair<StmtIndex, set<StmtIndex>>(stmt1, {}));
  }
  followsStarMap[stmt1].insert(stmt2);

  if (followedByStarMap.count(stmt2) == 0) {
    followedByStarMap.insert(pair<StmtIndex, set<StmtIndex>>(stmt2, {}));
  }
  followedByStarMap[stmt2].insert(stmt1);
}

bool FollowsStarTable::insertFollowsStar(StmtIndex stmt1, StmtIndex stmt2) {
  insertIntoMaps(stmt1, stmt2);

  //Adding follows*(prevStmts, stmt2)
  if (followedByStarMap.count(stmt1) > 0) {
    for (const StmtIndex& prevStmt : followedByStarMap[stmt1]) {
      insertIntoMaps(prevStmt, stmt2);
    }
  }

  //Adding follows*(stmt1, nextStmts)
  if (followsStarMap.count(stmt2) > 0) {
    for (const StmtIndex& nextStmt : followsStarMap[stmt2]) {
      insertIntoMaps(stmt1, nextStmt);
    }
  }

  return true;
}

set<StmtIndex> FollowsStarTable::getFollowingStarStmts(StmtIndex stmt) {
  return followsStarMap[stmt];
}

set<StmtIndex> FollowsStarTable::getPrecedingStarStmts(StmtIndex stmt) {
  return followedByStarMap[stmt];
}

//Follows(stmt1, stmt2)
bool FollowsStarTable::followsStar(StmtIndex stmt1, StmtIndex stmt2) {
  set<StmtIndex> nextIndexes = followsStarMap[stmt1];
  return nextIndexes.find(stmt2) != nextIndexes.end();
}

