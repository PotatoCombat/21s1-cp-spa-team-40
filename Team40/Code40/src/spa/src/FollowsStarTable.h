#pragma once

#include <map>
#include <set>
#include <Abstractions.h>

class FollowsStarTable {

public:
    FollowsStarTable();
    bool insertNextStatement(StatementIndex stmt1, StatementIndex stmt2);
    set<StatementIndex> getNextStatements(StatementIndex stmt);
    set<StatementIndex> getPreviousStatements(StatementIndex stmt);
    bool followsStar(StatementIndex stmt1, StatementIndex stmt2);

private:
    //stmt2 follows stmt1 (i.e. key comes before stmts in value)
    map<StatementIndex, set<StatementIndex>> followsStarTable;
    //stmt2 is followed by stmt1 (i.e. stmts in value comes before key)
    map<StatementIndex, set<StatementIndex>> followedByStarTable;
};

FollowsStarTable::FollowsStarTable() = default;

bool insertNextStatement(StatementIndex stmt1, StatementIndex stmt2) {
    if (followsStarTable.count(stmt1) <= 0) {
        followsStarTable.insert(pair<StatementIndex, set<StatementIndex>>(stmt1, {}));
    }
    followsStarTable[stmt1].insert(stmt2);

    if (followedByStarTable.count(stmt2) <= 0) {
        followedByStarTable.insert(pair<StatementIndex, set<StatementIndex>>(stmt2, {}));
    }
    followedByStarTable[stmt2].insert(stmt1);

    //Adding follows*(prevStmts, stmt2)
    if (followedByStarTable.count(stmt1) > 0) {
        for (const StatementIndex& prevStmt : followedByStarTable[stmt1]) {
            if (followsStarTable.count(prevStmt) <= 0) {
                followsStarTable.insert(pair<StatementIndex, set<StatementIndex>>(prevStmt, {}));
            }
            followsStarTable[prevStmt].insert(stmt2);

            if (followedByStarTable.count(stmt2) <= 0) {
                followedByStarTable.insert(pair<StatementIndex, set<StatementIndex>>(stmt2, {}));
            }
            followedByStarTable[stmt2].insert(prevStmt);
        }
    }

    //Adding follows*(stmt1, nextStmts)
    if (followsStarTable.count(stmt2) > 0) {
        for (const StatementIndex& nextStmt : followsStarTable[stmt2]) {
            if (followsStarTable.count(stmt1) <= 0) {
                followsStarTable.insert(pair<StatementIndex, set<StatementIndex>>(stmt1, {}));
            }
            followsStarTable[stmt1].insert(nextStmt);

            if (followedByStarTable.count(nextStmt) <= 0) {
                followedByStarTable.insert(pair<StatementIndex, set<StatementIndex>>(nextStmt, {}));
            }
            followedByStarTable[nextStmt].insert(stmt1);
        }
    }

    return true;
}

set<StatementIndex> getNextStatements(StatementIndex stmt) {
    return followsStarTable[stmt];
}

set<StatementIndex> getPreviousStatements(StatementIndex stmt) {
    return followedByStarTable[stmt];
}

bool followsStar(StatementIndex stmt1, StatementIndex stmt2) {
    set<StatementIndex> nextIndexes = followsStarTable[stmt1];
    if (nextIndexes.find(stmt2) != nextIndexes.end()) {
        return true;
    }
    return false;
}

