#include "PKBStub2.h"

// based on the Example procedure in Test40/sample_source
Iterator<StmtIndex> PKBStub2::getAllStmts() {
    vector<int> stmts{1, 2, 3};
    Iterator<StmtIndex> iter(stmts);
    return iter;
}

StmtIndex PKBStub2::getFollowingStmt(StmtIndex stmt) { return -1; }

StmtIndex PKBStub2::getPrecedingStmt(StmtIndex stmt) { return -1; }

bool PKBStub2::follows(StmtIndex stmt1, StmtIndex stmt2) { return false; }

StmtIndex PKBStub2::getParentStmt(StmtIndex stmt) { return -1; }

set<StmtIndex> PKBStub2::getChildStmts(StmtIndex stmt) {
    return set<StmtIndex>{};
}

bool PKBStub2::parent(StmtIndex stmt1, StmtIndex Stmt2) { return false; }
