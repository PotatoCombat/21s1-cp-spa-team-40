#include "FollowsTable.h"

FollowsTable::FollowsTable() = default;

bool FollowsTable::insertFollows(StmtIndex stmt1, StmtIndex stmt2) {
  followsMap[stmt1] = stmt2;
  followedByMap[stmt2] = stmt1;
  return true;
}
StmtIndex FollowsTable::getFollowingStmt(StmtIndex stmt) {
  return followsMap.at(stmt);
}

StmtIndex FollowsTable::getPrecedingStmt(StmtIndex stmt) {
  return followedByMap.at(stmt);
}

//Follows*(stmt1, stmt2)
bool FollowsTable::follows(StmtIndex stmt1, StmtIndex stmt2) {
  StmtIndex followsIndex = followsMap.at(stmt1);
  return followsIndex == stmt2;
}

