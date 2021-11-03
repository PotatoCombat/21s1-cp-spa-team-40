#pragma once

#include "pkb/PKB.h"

using namespace std;

class PKBStub2 : public PKB {
public:
    Iterator<StmtIndex> getAllStmts() override;

    StmtIndex getFollowingStmt(const StmtIndex &stmt) override;
    StmtIndex getPrecedingStmt(const StmtIndex &stmt) override;
    bool follows(const StmtIndex &stmt1, const StmtIndex &stmt2) override;

    StmtIndex getParentStmt(const StmtIndex &stmtstmt) override;
    set<StmtIndex> getChildStmts(const StmtIndex &stmtstmt) override;
    bool parent(const StmtIndex &stmt1, const StmtIndex &stmt2) override;
};
