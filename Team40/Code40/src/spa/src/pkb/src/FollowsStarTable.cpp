#include "pkb/FollowsStarTable.h"

using namespace std;

FollowsStarTable::FollowsStarTable() = default;

void FollowsStarTable::insertFollowsStar(StmtIndex stmt1, StmtIndex stmt2) {
    insertIntoMaps(stmt1, stmt2);

    // Adding follows*(prevStmts, stmt2)
    auto search1 = followedByStarMap.find(stmt1);
    if (search1 != followedByStarMap.end()) {
        for (const StmtIndex &prevStmt : search1->second) {
            insertIntoMaps(prevStmt, stmt2);
        }
    }

    // Adding follows*(stmt1, nextStmts)
    auto search2 = followsStarMap.find(stmt2);
    if (search2 != followsStarMap.end()) {
        for (const StmtIndex &nextStmt : search2->second) {
            insertIntoMaps(stmt1, nextStmt);
        }
    }
}

set<StmtIndex> FollowsStarTable::getFollowingStarStmts(StmtIndex stmt) {
    auto result = followsStarMap.find(stmt);
    if (result == followsStarMap.end())
    {
        return {};
    }
    return result->second;
}

set<StmtIndex> FollowsStarTable::getPrecedingStarStmts(StmtIndex stmt) {
    auto result = followedByStarMap.find(stmt);
    if (result == followedByStarMap.end())
    {
        return {};
    }
    return result->second;
}

// Follows(stmt1, stmt2)
bool FollowsStarTable::followsStar(StmtIndex stmt1, StmtIndex stmt2) {
    auto result = followsStarMap.find(stmt1);
    if (result == followsStarMap.end())
    {
        return false;
    }
    set<StmtIndex> followsIndices = result->second;
    return followsIndices.find(stmt2) != followsIndices.end();
}

void FollowsStarTable::insertIntoMaps(StmtIndex stmt1, StmtIndex stmt2) {
    auto search1 = followsStarMap.find(stmt1);
    if (search1 == followsStarMap.end()) {
        followsStarMap[stmt1] = { stmt2 };
    }
    else {
        search1->second.insert(stmt2);
    }

    auto search2 = followedByStarMap.find(stmt2);
    if (search2 == followedByStarMap.end()) {
        followedByStarMap[stmt2] = { stmt1 };
    }
    else {
        search2->second.insert(stmt1);
    }
}
