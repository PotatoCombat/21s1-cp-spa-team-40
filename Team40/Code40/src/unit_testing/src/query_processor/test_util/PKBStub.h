#pragma once

#include <set>
#include <vector>

#include "pkb/PKB.h"

using namespace std;

class PKBStub : public PKB {
public:
    Iterator<StmtIndex> getAllStmts();
    Iterator<VarName> getAllVars();
    Iterator<ConstName> getAllConsts();

    Iterator<StmtIndex> getAllStmts(const StatementType &stmtType);

    StatementType getStmtType(const StmtIndex &stmt);

    StmtIndex getFollowingStmt(const StmtIndex &stmt);
    StmtIndex getPrecedingStmt(const StmtIndex &stmt);
    bool follows(const StmtIndex &stmt1, const StmtIndex &stmt2);

    set<StmtIndex> getFollowingStarStmts(const StmtIndex &stmt);
    set<StmtIndex> getPrecedingStarStmts(const StmtIndex &stmt);
    bool followsStar(const StmtIndex &stmt1, const StmtIndex &stmt2);

    StmtIndex getParentStmt(const StmtIndex &stmt);
    set<StmtIndex> getChildStmts(const StmtIndex &stmt);
    bool parent(const StmtIndex &stmt1, const StmtIndex &stmt2);

    set<StmtIndex> getParentStarStmts(const StmtIndex &stmt);
    set<StmtIndex> getChildStarStmts(const StmtIndex &stmt);
    bool parentStar(const StmtIndex &stmt1, const StmtIndex &stmt2);

    set<StmtIndex> getStmtsUsingVar(const VarName &var);
    set<VarName> getVarsUsedByStmt(const StmtIndex &stmt);
    bool stmtUses(const StmtIndex &stmt, const VarName &var);

    set<StmtIndex> getStmtsModifyingVar(const VarName &var);
    set<VarName> getVarsModifiedByStmt(const StmtIndex &stmt);
    bool stmtModifies(const StmtIndex &stmt, const VarName &var);

    VarName getPrintVariable(const StmtIndex &printStmt);
    VarName getReadVariable(const StmtIndex &readStmt);
    ProcName getCallProcedure(const StmtIndex &callStmt);
};
