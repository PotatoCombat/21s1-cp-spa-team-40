#pragma once

#include <map>
#include <set>
#include <Abstractions.h>

class FollowsStarTable {

public:
    FollowsStarTable();
    bool insertFollowsStar(StatementIndex stmt1, StatementIndex stmt2);
    set<StatementIndex> getFollowingStarStatements(StatementIndex stmt);
    set<StatementIndex> getPrecedingStarStatements(StatementIndex stmt);
    bool followsStar(StatementIndex stmt1, StatementIndex stmt2);

private:
    //stmt2 follows stmt1 (i.e. key comes before stmts in value)
    map<StatementIndex, set<StatementIndex>> followsStarMap;
    //stmt2 is followed by stmt1 (i.e. stmts in value comes before key)
    map<StatementIndex, set<StatementIndex>> followedByStarMap;
    void insertIntoMaps(StatementIndex stmt1, StatementIndex stmt2);
};

FollowsStarTable::FollowsStarTable() = default;

void FollowsStarTable::insertIntoMaps(StatementIndex stmt1, StatementIndex stmt2) {
    if (followsStarMap.count(stmt1) <= 0) {
        followsStarMap.insert(pair<StatementIndex, set<StatementIndex>>(stmt1, {}));
    }
    followsStarMap[stmt1].insert(stmt2);

    if (followedByStarMap.count(stmt2) <= 0) {
        followedByStarMap.insert(pair<StatementIndex, set<StatementIndex>>(stmt2, {}));
    }
    followedByStarMap[stmt2].insert(stmt1);
}

bool FollowsStarTable::insertFollowsStar(StatementIndex stmt1, StatementIndex stmt2) {
    insertIntoMaps(stmt1, stmt2);

    //Adding follows*(prevStmts, stmt2)
    if (followedByStarMap.count(stmt1) > 0) {
        for (const StatementIndex& prevStmt : followedByStarMap[stmt1]) {
            insertIntoMaps(prevStmt, stmt2);
        }
    }

    //Adding follows*(stmt1, nextStmts)
    if (followsStarMap.count(stmt2) > 0) {
        for (const StatementIndex& nextStmt : followsStarMap[stmt2]) {
            insertIntoMaps(stmt1, nextStmt);
        }
    }

    return true;
}

set<StatementIndex> FollowsStarTable::getFollowingStarStatements(StatementIndex stmt) {
    return followsStarMap[stmt];
}

set<StatementIndex> FollowsStarTable::getPrecedingStarStatements(StatementIndex stmt) {
    return followedByStarMap[stmt];
}

//Follows(stmt1, stmt2)
bool FollowsStarTable::followsStar(StatementIndex stmt1, StatementIndex stmt2) {
    set<StatementIndex> nextIndexes = followsStarMap[stmt1];
    if (nextIndexes.find(stmt2) != nextIndexes.end()) {
        return true;
    }
    return false;
}

