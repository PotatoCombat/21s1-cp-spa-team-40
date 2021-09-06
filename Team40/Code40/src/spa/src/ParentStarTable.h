#pragma once

#include <map>
#include <set>
#include <Abstractions.h>

class ParentStarTable {

public:
    ParentStarTable();
    bool insertParentStar(StatementIndex stmt1, StatementIndex stmt2);
    set<StatementIndex> getParentStarStatements(StatementIndex stmt);
    set<StatementIndex> getChildStarStatements(StatementIndex stmt);
    bool parentStar(StatementIndex stmt1, StatementIndex stmt2);

private:
    //value is parent of key (value contains all parents of key)
    map<StatementIndex, set<StatementIndex>> parentStarMap;
    //value is child of key (value contains all children of key)
    map<StatementIndex, set<StatementIndex>> childStarMap;
    void insertIntoMaps(StatementIndex stmt1, StatementIndex stmt2);
};

ParentStarTable::ParentStarTable() = default;

//NOTE: stmt2 is parent of stmt1
void ParentStarTable::insertIntoMaps(StatementIndex stmt1, StatementIndex stmt2) {
    if (parentStarMap.count(stmt1) <= 0) {
        parentStarMap.insert(pair<StatementIndex, set<StatementIndex>>(stmt1, {}));
    }
    parentStarMap[stmt1].insert(stmt2);

    if (childStarMap.count(stmt2) <= 0) {
        childStarMap.insert(pair<StatementIndex, set<StatementIndex>>(stmt2, {}));
    }
    childStarMap[stmt2].insert(stmt1);
}

//NOTE: stmt2 is parent of stmt1
bool ParentStarTable::insertParentStar(StatementIndex stmt1, StatementIndex stmt2) {
    insertIntoMaps(stmt1, stmt2);

    //Adding parent*(prevStmts, stmt2)
    if (childStarMap.count(stmt1) > 0) {
        for (const StatementIndex& prevStmt : childStarMap[stmt1]) {
            insertIntoMaps(prevStmt, stmt2);
        }
    }

    //Adding parent*(stmt1, nextStmts)
    if (parentStarMap.count(stmt2) > 0) {
        for (const StatementIndex& nextStmt : parentStarMap[stmt2]) {
            insertIntoMaps(stmt1, nextStmt);
        }
    }

    return true;
}

set<StatementIndex> ParentStarTable::getParentStarStatements(StatementIndex stmt) {
    return parentStarMap[stmt];
}

set<StatementIndex> ParentStarTable::getChildStarStatements(StatementIndex stmt) {
    return childStarMap[stmt];
}

//Parent*(stmt1, stmt2)
bool ParentStarTable::parentStar(StatementIndex stmt1, StatementIndex stmt2) {
    set<StatementIndex> parentIndexes = parentStarMap[stmt1];
    if (nextIndexes.find(stmt2) != nextIndexes.end()) {
        return true;
    }
    return false;
}


