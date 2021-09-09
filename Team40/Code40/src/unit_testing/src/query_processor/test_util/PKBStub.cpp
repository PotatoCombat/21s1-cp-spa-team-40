#include "PKBStub.h"

// based on the Example procedure in Test40/sample_source
Iterator<StmtIndex> PKBStub::getAllStmts() {
    vector<int> stmts{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    Iterator<StmtIndex> iter(stmts);
    return iter;
}

StmtIndex PKBStub::getFollowingStmt(StmtIndex stmt) {
    vector<int> followingStmts{2, 3, 4, 12, 6, 9, -1, -1, 10, 11, -1, -1};
    return followingStmts[stmt - 1];
}

StmtIndex PKBStub::getPrecedingStmt(StmtIndex stmt) {
    vector<int> precedingStmts{-1, 1, 2, 3, -1, 5, -1, -1, 6, 9, 10, 4};
    return precedingStmts[stmt - 1];
}

bool PKBStub::follows(StmtIndex stmt1, StmtIndex stmt2) {
    return getFollowingStmt(stmt1) == stmt2;
}