#include "PKBStub2.h"

// based on the Example procedure in Test40/sample_source
Iterator<StmtIndex> PKBStub2::getAllStmts() {
    vector<int> stmts{1, 2, 3};
    Iterator<StmtIndex> iter(stmts);
    return iter;
}

StmtIndex PKBStub2::getFollowingStmt(const StmtIndex &stmt) { return InvalidIndex; }

StmtIndex PKBStub2::getPrecedingStmt(const StmtIndex &stmt) { return InvalidIndex; }

bool PKBStub2::follows(const StmtIndex &stmt1, const StmtIndex &stmt2) { return false; }

StmtIndex PKBStub2::getParentStmt(const StmtIndex &stmt) { return InvalidIndex; }

set<StmtIndex> PKBStub2::getChildStmts(const StmtIndex &stmt) {
    return { };
}

bool PKBStub2::parent(const StmtIndex &stmt1, const StmtIndex &stmt2) { return false; }
