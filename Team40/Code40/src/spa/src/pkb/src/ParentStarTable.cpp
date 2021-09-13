#include "pkb/ParentStarTable.h"

ParentStarTable::ParentStarTable() = default;

// NOTE: stmt1 is parent of stmt2
void ParentStarTable::insertParentStar(Statement *stmt1, Statement *stmt2) {
    insertIntoMaps(stmt1->getIndex(), stmt2->getIndex());

    // Adding parent*(parentStmts, stmt2)
    auto search1 = parentStarMap.find(stmt1->getIndex());
    if (search1 != parentStarMap.end()) {
        for (const StmtIndex &parentStmt : search1->second) {
            insertIntoMaps(parentStmt, stmt2->getIndex());
        }
    }

    // Adding parent*(stmt1, childStmts)
    auto search2 = childStarMap.find(stmt2->getIndex());
    if (search2 != childStarMap.end()) {
        for (const StmtIndex &childStmt : search2->second) {
            insertIntoMaps(stmt1->getIndex(), childStmt);
        }
    }
}

set<StmtIndex> ParentStarTable::getParentStarStmts(StmtIndex stmt) {
    auto result = parentStarMap.find(stmt);
    if (result == parentStarMap.end()) {
        return {};
    }
    return result->second;
}

set<StmtIndex> ParentStarTable::getChildStarStmts(StmtIndex stmt) {
    auto result = childStarMap.find(stmt);
    if (result == childStarMap.end()) {
        return {};
    }
    return result->second;
}

// NOTE: stmt1 is parent of stmt2
bool ParentStarTable::parentStar(StmtIndex stmt1, StmtIndex stmt2) {
    auto result = parentStarMap.find(stmt2);
    if (result == parentStarMap.end())
    {
        return false;
    }
    set<StmtIndex> parentIndices = result->second;
    return parentIndices.find(stmt1) != parentIndices.end();
}

// NOTE: stmt1 is parent of stmt2
void ParentStarTable::insertIntoMaps(StmtIndex stmt1, StmtIndex stmt2) {
    auto search1 = parentStarMap.find(stmt2);
    if (search1 == parentStarMap.end()) {
        parentStarMap[stmt2] = { stmt1 };
    }
    else {
        search1->second.insert(stmt1);
    }

    auto search2 = childStarMap.find(stmt1);
    if (search2 == childStarMap.end()) {
        childStarMap[stmt1] = { stmt2 };
    }
    else {
        search2->second.insert(stmt2);
    }
}
