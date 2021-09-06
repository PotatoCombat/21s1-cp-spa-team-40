#pragma once

#include <map>
#include <Abstractions.h>

class FollowsTable {

public:
    FollowsTable();
    bool insertFollows(StatementIndex stmt1, StatementIndex stmt2);
    StatementIndex getFollowingStatement(StatementIndex stmt);
    StatementIndex getPrecedingStatement(StatementIndex stmt);
    bool follows(StatementIndex stmt1, StatementIndex stmt2);

private:
    //value follows key
    map<StatementIndex, StatementIndex> followsMap;
    //key follows value (value is followed by key)
    map<StatementIndex, StatementIndex> followedByMap;
};

FollowsTable::FollowsTable() = default;

bool FollowsTable::insertFollows(StatementIndex stmt1, StatementIndex stmt2) {
    followsMap[stmt1] = stmt2;
    followedByMap[stmt2] = stmt1;
    return true;
}

StatementIndex FollowsTable::getFollowingStatement(StatementIndex stmt) {
    return followsMap.at(stmt);
}

StatementIndex FollowsTable::getPrecedingStatement(StatementIndex stmt) {
    return followedByMap.at(stmt);
}

//Follows*(stmt1, stmt2)
bool FollowsTable::follows(StatementIndex stmt1, StatementIndex stmt2) {
    StatementIndex followsIndex = followsMap.at(stmt1);
    if (followsIndex == stmt2) {
        return true;
    }
    return false;
}

