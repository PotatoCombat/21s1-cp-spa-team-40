#include "pkb/ParentTable.h"

ParentTable::ParentTable() = default;

// NOTE: stmt1 is parent of stmt2
void ParentTable::insertParent(StmtIndex stmt1, StmtIndex stmt2) {
    parentMap[stmt2] = stmt1;

    auto search = childMap.find(stmt1);
    if (search == childMap.end()) {
        childMap[stmt1] = {stmt2};
    } else {
        search->second.insert(stmt2);
    }
}

StmtIndex ParentTable::getParentStmt(StmtIndex stmt) {
    auto result = parentMap.find(stmt);
    if (result == parentMap.end()) {
        return InvalidIndex;
    }
    return result->second;
}

set<StmtIndex> ParentTable::getChildStmts(StmtIndex stmt) {
    auto result = childMap.find(stmt);
    if (result == childMap.end()) {
        return {};
    }
    return result->second;
}

// NOTE: stmt1 is parent of stmt2
bool ParentTable::parent(StmtIndex stmt1, StmtIndex stmt2) {
    auto result = parentMap.find(stmt2);
    if (result == parentMap.end()) {
        return false;
    }
    StmtIndex parentIndex = result->second;
    return parentIndex == stmt1;
}
