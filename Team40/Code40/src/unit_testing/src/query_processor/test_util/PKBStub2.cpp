#include "PKBStub2.h"

// based on the Example procedure in Test40/sample_source
Iterator<StmtIndex> PKBStub2::getAllStmts() {
    vector<int> stmts{1, 2, 3};
    Iterator<StmtIndex> iter(stmts);
    return iter;
}

StmtIndex PKBStub2::getFollowingStmt(StmtIndex stmt) {
    vector<int> followingStmts{-1, -1, -1};
    return followingStmts[stmt - 1];
}

StmtIndex PKBStub2::getPrecedingStmt(StmtIndex stmt) {
    vector<int> precedingStmts{-1, -1, -1};
    return precedingStmts[stmt - 1];
}

bool PKBStub2::follows(StmtIndex stmt1, StmtIndex stmt2) {
    return getFollowingStmt(stmt1) == stmt2;
}
