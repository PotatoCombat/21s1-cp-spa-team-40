#include "pkb/BranchInTable.h"

BranchInTable::BranchInTable() = default;

void BranchInTable::insertBranchIn(Statement *stmt1, Statement *stmt2) {
    branchInToMap[stmt1->getIndex()] = stmt2->getIndex();

    auto search = branchInFromMap.find(stmt2->getIndex());
    if (search == branchInFromMap.end()) {
        branchInFromMap[stmt2->getIndex()] = {stmt1->getIndex()};
    } else {
        search->second.insert(stmt1->getIndex());
    }
}

StmtIndex BranchInTable::getBranchInToStmt(StmtIndex stmt) {
    auto result = branchInToMap.find(stmt);
    if (result == branchInToMap.end()) {
        return InvalidIndex;
    }
    return result->second;
}

set<StmtIndex> BranchInTable::getBranchInFromStmts(StmtIndex stmt) {
    auto result = branchInFromMap.find(stmt);
    if (result == branchInFromMap.end()) {
        return {};
    }
    return result->second;
}

bool BranchInTable::branchIn(StmtIndex stmt1, StmtIndex stmt2) {
    auto result = branchInToMap.find(stmt1);
    if (result == branchInToMap.end()) {
        return false;
    }
    StmtIndex branchInIndex = result->second;
    return branchInIndex == stmt2;
}
