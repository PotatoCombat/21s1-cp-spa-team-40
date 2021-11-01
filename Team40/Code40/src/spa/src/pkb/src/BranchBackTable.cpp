#include "pkb/BranchBackTable.h"

BranchBackTable::BranchBackTable() = default;

void BranchBackTable::insertBranchBack(Statement *stmt1, Statement *stmt2) {
    branchBackFromMap[stmt1->getIndex()] = stmt2->getIndex();

    auto search = branchBackToMap.find(stmt1->getIndex());
    if (search == branchBackToMap.end()) {
        branchBackToMap[stmt1->getIndex()] = {stmt2->getIndex()};
    } else {
        search->second.insert(stmt2->getIndex());
    }
}

StmtIndex BranchBackTable::getBranchBackFromStmt(StmtIndex stmt) {
    auto result = branchBackFromMap.find(stmt);
    if (result == branchBackFromMap.end()) {
        return InvalidIndex;
    }
    return result->second;
}

set<StmtIndex> BranchBackTable::getBranchBackToStmts(StmtIndex stmt) {
    auto result = branchBackToMap.find(stmt);
    if (result == branchBackToMap.end()) {
        return {};
    }
    return result->second;
}

bool BranchBackTable::branchBack(StmtIndex stmt1, StmtIndex stmt2) {
    auto result = branchBackFromMap.find(stmt2);
    if (result == branchBackFromMap.end()) {
        return false;
    }
    StmtIndex branchBackIndex = result->second;
    return branchBackIndex == stmt1;
}
