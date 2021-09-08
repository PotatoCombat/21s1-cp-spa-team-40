#pragma once

//#include "Abstractions.h"
#include "EntityTable.h"
#include "FollowsStarTable.h"
#include "FollowsTable.h"
#include "Iterator.h"
#include "ModifiesTable.h"
#include "ParentStarTable.h"
#include "ParentTable.h"
#include "StatementTable.h"
#include "UsesTable.h"
#include "common/model/Procedure.h"
#include "common/model/statement/Statement.h"

using namespace std;

class PKB {
public:
    virtual Iterator<VarIndex> getAllVars();
    virtual Iterator<ConstIndex> getAllConsts();
    virtual Iterator<ProcIndex> getAllProcs();

    virtual Iterator<StmtIndex> getAllStmts();

    virtual VarIndex insertVar(Var *variable);
    virtual ConstIndex insertConst(Const *constant);
    virtual ProcIndex insertProc(Procedure *procedure);

    virtual StmtIndex insertStmt(Statement *statement);

    // stmt2 follows stmt1. This also adds to FollowStarTable
    virtual bool insertFollows(StmtIndex stmt1, StmtIndex stmt2);
    virtual StmtIndex getFollowingStmt(StmtIndex stmt);
    virtual StmtIndex getPrecedingStmt(StmtIndex stmt);
    // checks if stmt2 follows stmt1
    virtual bool follows(StmtIndex stmt1, StmtIndex stmt2);

    virtual set<StmtIndex> getFollowingStarStmts(StmtIndex stmt);
    virtual set<StmtIndex> getPrecedingStarStmts(StmtIndex stmt);
    // checks if stmt2 follows* stmt1
    virtual bool followsStar(StmtIndex stmt1, StmtIndex stmt2);

    // stmt2 is parent of stmt1. This also adds to ParentStarTable
    virtual bool insertParent(StmtIndex stmt1, StmtIndex stmt2);
    virtual StmtIndex getParentStmt(StmtIndex stmt);
    virtual StmtIndex getChildStmt(StmtIndex stmt);
    // checks if stmt2 is parent of stmt1
    virtual bool parent(StmtIndex stmt1, StmtIndex stmt2);

    virtual set<StmtIndex> getParentStarStmts(StmtIndex stmt);
    virtual set<StmtIndex> getChildStarStmts(StmtIndex stmt);
    // checks if stmt2 is parent* of stmt1
    virtual bool parentStar(StmtIndex stmt1, StmtIndex stmt2);

    virtual bool insertStmtModifyingVar(StmtIndex stmt, VarIndex var);
    virtual bool insertProcModifyingVar(ProcIndex proc, VarIndex var);
    virtual set<VarIndex> getVarsModifiedByStmt(StmtIndex stmt);
    virtual set<StmtIndex> getStmtsModifyingVar(VarIndex var);
    virtual set<VarIndex> getVarsModifiedByProc(ProcIndex proc);
    virtual set<ProcIndex> getProcsModifyingVar(VarIndex var);
    virtual bool stmtModifies(StmtIndex stmt, VarIndex var);
    virtual bool procModifies(ProcIndex proc, VarIndex var);

    virtual bool insertStmtUsingVar(StmtIndex stmt, VarIndex var);
    virtual bool insertProcUsingVar(ProcIndex proc, VarIndex var);
    virtual set<VarIndex> getVarsUsedByStmt(StmtIndex stmt);
    virtual set<StmtIndex> getStmtsUsingVar(VarIndex var);
    virtual set<VarIndex> getVarsUsedByProc(ProcIndex proc);
    virtual set<ProcIndex> getProcsUsingVar(VarIndex var);
    virtual bool stmtUses(StmtIndex stmt, VarIndex var);
    virtual bool procUses(ProcIndex proc, VarIndex var);

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
