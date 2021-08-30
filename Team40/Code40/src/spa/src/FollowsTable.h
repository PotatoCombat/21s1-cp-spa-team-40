#pragma once

#include <map>
#include <Abstractions.h>

class FollowsTable {

public:
    FollowsTable();
    bool insertNextStatement(StatementIndex stmt1, StatementIndex stmt2);
    StatementIndex getNextStatement(StatementIndex stmt);
    StatementIndex getPreviousStatement(StatementIndex stmt);
    bool follows(StatementIndex stmt1, StatementIndex stmt2);

private:
    //stmt2 follows stmt1 (i.e. key comes before value)
    map<StatementIndex, StatementIndex> followsTable;
    //stmt2 is followed by stmt1 (i.e. value comes before key)
    map<StatementIndex, StatementIndex> followedByTable;
};

FollowsTable::FollowsTable() = default;

bool insertNextStatement(StatementIndex stmt1, StatementIndex stmt2) {
    followsTable[stmt1] = stmt2;
    followedByTable[stmt2] = stmt1;
    return true;
}

StatementIndex getNextStatement(StatementIndex stmt) {
    return followsTable.at(stmt);
}

StatementIndex getPreviousStatement(StatementIndex stmt) {
    return followedByTable.at(stmt);
}

bool follows(StatementIndex stmt1, StatementIndex stmt2) {
    StatementIndex nextIndex = followsTable.at(stmt1);
    if (nextIndex == stmt2) {
        return true;
    }
    return false;
}

