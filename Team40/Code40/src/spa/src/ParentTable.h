#pragma once

#include <map>
#include <Abstractions.h>

class ParentTable {

public:
    ParentTable();
    bool insertParent(StatementIndex stmt1, StatementIndex stmt2);
    StatementIndex getParentStatement(StatementIndex stmt);
    StatementIndex getChildStatement(StatementIndex stmt);
    bool parent(StatementIndex stmt1, StatementIndex stmt2);

private:
    //value is parent of key
    map<StatementIndex, StatementIndex> parentMap;
    //key is parent of value (value is child of key)
    map<StatementIndex, StatementIndex> childMap;
};

ParentTable::ParentTable() = default;

//NOTE: stmt2 is parent of stmt1
bool ParentTable::insertParent(StatementIndex stmt1, StatementIndex stmt2) {
    parentMap[stmt1] = stmt2;
    childMap[stmt2] = stmt1;
    return true;
}

StatementIndex ParentTable::getParentStatement(StatementIndex stmt) {
    return parentMap.at(stmt);
}

StatementIndex ParentTable::getChildStatement(StatementIndex stmt) {
    return childMap.at(stmt);
}

//Parent(stmt1, stmt2)
bool ParentTable::parent(StatementIndex stmt1, StatementIndex stmt2) {
    StatementIndex parentIndex = parentMap.at(stmt1);
    if (parentIndex == stmt2) {
        return true;
    }
    return false;
}

