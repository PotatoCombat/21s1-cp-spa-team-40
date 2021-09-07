#pragma once

//#include "Abstractions.h"
#include "EntityTable.h"
#include "Iterator.h"
#include "StatementTable.h"
#include "FollowsTable.h"
#include "FollowsStarTable.h"
#include "ParentTable.h"
#include "ParentStarTable.h"
#include "ModifiesTable.h"
#include "UsesTable.h"

using namespace std;

class PKB {
public:
    Iterator<VarIndex> getAllVars();
    Iterator<ConstIndex> getAllConsts();
    Iterator<ProcIndex> getAllProcs();

    Iterator<StmtIndex> getAllStmts();

    VarIndex insertVar(Var variable);
    ConstIndex insertConst(Const constant);
    ProcIndex insertProc(Procedure procedure);

    StmtIndex insertStmt(Statement statement);

    //stmt2 follows stmt1. This also adds to FollowStarTable
    bool insertFollows(StmtIndex stmt1, StmtIndex stmt2);
    StmtIndex getFollowingStmt(StmtIndex stmt);
    StmtIndex getPrecedingStmt(StmtIndex stmt);
    //checks if stmt2 follows stmt1
    bool follows(StmtIndex stmt1, StmtIndex stmt2);

    set<StmtIndex> getFollowingStarStmts(StmtIndex stmt);
    set<StmtIndex> getPrecedingStarStmts(StmtIndex stmt);
    //checks if stmt2 follows* stmt1
    bool followsStar(StmtIndex stmt1, StmtIndex stmt2);

    //stmt2 is parent of stmt1. This also adds to ParentStarTable
    bool insertParent(StmtIndex stmt1, StmtIndex stmt2);
    StmtIndex getParentStmt(StmtIndex stmt);
    StmtIndex getChildStmt(StmtIndex stmt);
    //checks if stmt2 is parent of stmt1
    bool parent(StmtIndex stmt1, StmtIndex stmt2);

    set<StmtIndex> getParentStarStmts(StmtIndex stmt);
    set<StmtIndex> getChildStarStmts(StmtIndex stmt);
    //checks if stmt2 is parent* of stmt1
    bool parentStar(StmtIndex stmt1, StmtIndex stmt2);

    bool insertStmtModifyingVar(StmtIndex stmt, VarIndex var);
    bool insertProcModifyingVar(ProcIndex proc, VarIndex var);
    set<VarIndex> getVarsModifiedByStmt(StmtIndex stmt);
    set<StmtIndex> getStmtsModifyingVar(VarIndex var);
    set<VarIndex> getVarsModifiedByProc(ProcIndex proc);
    set<ProcIndex> getProcsModifyingVar(VarIndex var);
    bool stmtModifies(StmtIndex stmt, VarIndex var);
    bool procModifies(ProcIndex proc, VarIndex var);

    bool insertStmtUsingVar(StmtIndex stmt, VarIndex var);
    bool insertProcUsingVar(ProcIndex proc, VarIndex var);
    set<VarIndex> getVarsUsedByStmt(StmtIndex stmt);
    set<StmtIndex> getStmtsUsingVar(VarIndex var);
    set<VarIndex> getVarsUsedByProc(ProcIndex proc);
    set<ProcIndex> getProcsUsingVar(VarIndex var);
    bool stmtUses(StmtIndex stmt, VarIndex var);
    bool procUses(ProcIndex proc, VarIndex var);

private:
    typedef EntityTable<Var, VarIndex> VarTable;
    typedef EntityTable<Const, ConstIndex> ConstTable;
    typedef EntityTable<Procedure, ProcIndex> ProcedureTable;

    typedef StatementTable<Statement, StmtIndex> StatementTable;

    VarTable varTable;
    ConstTable constTable;
    ProcedureTable procTable;

    StatementTable statementTable;

    FollowsTable followsTable;
    FollowsStarTable followsStarTable;
    ParentTable parentTable;
    ParentStarTable parentStarTable;
    ModifiesTable modifiesTable;
    UsesTable usesTable;
};
