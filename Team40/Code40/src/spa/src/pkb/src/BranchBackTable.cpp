#include "pkb/BranchBackTable.h"

BranchBackTable::BranchBackTable() = default;

void BranchBackTable::insertBranchBack(Statement *stmt1, Statement *stmt2) {
    auto search = branchBackToMap.find(stmt1->getIndex());
    if (search == branchBackToMap.end()) {
        branchBackToMap[stmt1->getIndex()] = {stmt2->getIndex()};
    } else {
        search->second.insert(stmt2->getIndex());
    }

    search = branchBackFromMap.find(stmt2->getIndex());
    if (search == branchBackFromMap.end()) {
        branchBackFromMap[stmt2->getIndex()] = {stmt1->getIndex()};
    } else {
        search->second.insert(stmt1->getIndex());
    }
}

set<StmtIndex> BranchBackTable::getBranchBackFromStmts(StmtIndex stmt) {
    auto result = branchBackFromMap.find(stmt);
    if (result == branchBackFromMap.end()) {
        return {};
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
    set<StmtIndex> branchBackIndices = result->second;
    return branchBackIndices.find(stmt1) != branchBackIndices.end();
}
