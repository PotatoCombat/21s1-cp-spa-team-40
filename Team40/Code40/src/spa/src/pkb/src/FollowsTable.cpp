#include "pkb/FollowsTable.h"

FollowsTable::FollowsTable() = default;

void FollowsTable::insertFollows(Statement *stmt1, Statement *stmt2) {
    followsMap[stmt1->getIndex()] = stmt2->getIndex();
    followedByMap[stmt2->getIndex()] = stmt1->getIndex();
}

StmtIndex FollowsTable::getFollowingStmt(StmtIndex stmt) {
    auto result = followsMap.find(stmt);
    if (result == followsMap.end())
    {
        return InvalidIndex;
    }
    return result->second;
}

StmtIndex FollowsTable::getPrecedingStmt(StmtIndex stmt) {
    auto result = followedByMap.find(stmt);
    if (result == followedByMap.end())
    {
        return InvalidIndex;
    }
    return result->second;
}

// Follows*(stmt1, stmt2)
bool FollowsTable::follows(StmtIndex stmt1, StmtIndex stmt2) {
    auto result = followsMap.find(stmt1);
    if (result == followsMap.end())
    {
        return false;
    }
    StmtIndex followsIndex = result->second;
    return followsIndex == stmt2;
}
