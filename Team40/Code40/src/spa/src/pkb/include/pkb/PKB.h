#pragma once

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
#include "common/model/Variable.h"
#include "common/model/ConstantValue.h"
#include "common/model/statement/Statement.h"

using namespace std;

class PKB {
public:
    // =========================================================================
    // Source Processor
    // =========================================================================

    virtual ProcIndex insertProc(Procedure *procedure);
    virtual StmtIndex insertStmt(Statement *statement);
    virtual VarIndex insertVar(Variable *variable);
    virtual ConstIndex insertConst(ConstantValue *constant);

    /// Stores the relationship Follows(stmt1, stmt2), and updates *
    /// relationships.
    virtual void insertFollows(Statement *precedingStmt,
                               Statement *followingStmt);

    /// Stores the relationship Parent(stmt1, stmt2), and updates *
    /// relationships.
    virtual void insertParent(StmtIndex parentStmt, StmtIndex childStmt);

    /// Stores the relationship Modifies(proc, var).
    virtual void insertProcModifyingVar(ProcIndex proc, VarIndex var);

    /// Stores the relationship Modifies(stmt, var).
    virtual void insertStmtModifyingVar(StmtIndex stmt, VarIndex var);

    /// Stores the relationship Uses(proc, var).
    virtual void insertProcUsingVar(ProcIndex proc, VarIndex var);

    /// Stores the relationship Uses(stmt, var).
    virtual void insertStmtUsingVar(StmtIndex stmt, VarIndex var);

    // =========================================================================
    // Query Processor
    // =========================================================================

    virtual Iterator<ProcIndex> getAllProcs();
    virtual Iterator<StmtIndex> getAllStmts();
    virtual Iterator<StmtIndex> getAllStmts(StatementType type);
    virtual Iterator<VarIndex> getAllVars();
    virtual Iterator<ConstIndex> getAllConsts();

    // Follows =================================================================

    /// Selects s such that Follows(stmt, s).
    /// \return stmt#no that fits the relationship, or InvalidIndex if there
    /// is none.
    virtual StmtIndex getFollowingStmt(StmtIndex stmt);

    /// Selects s such that Follows*(stmt, s).
    /// \return all stmt#no that fit the relationship, or an empty set if there
    /// are none.
    virtual set<StmtIndex> getFollowingStarStmts(StmtIndex stmt);

    /// Selects s such that Follows(s, stmt).
    /// \return stmt#no that fits the relationship, or InvalidIndex if there
    /// is none.
    virtual StmtIndex getPrecedingStmt(StmtIndex stmt);

    /// Selects s such that Follows*(s, stmt).
    /// \return all stmt#no that fit the relationship, or an empty set if there
    /// are none.
    virtual set<StmtIndex> getPrecedingStarStmts(StmtIndex stmt);

    /// Selects BOOLEAN such that Follows(stmt1, stmt2).
    virtual bool follows(StmtIndex precedingStmt, StmtIndex followingStmt);

    /// Selects BOOLEAN such that Follows*(stmt1, stmt2).
    virtual bool followsStar(StmtIndex precedingStmt, StmtIndex followingStmt);

    // Parent ==================================================================

    /// Selects s such that Parent(s, stmt).
    /// \return stmt#no that fits the relationship, or InvalidIndex if there
    /// is none.
    virtual StmtIndex getParentStmt(StmtIndex stmt);

    /// Selects s such that Parent*(s, stmt).
    /// \return all stmt#no that fit the relationship, or an empty set if there
    /// are none.
    virtual set<StmtIndex> getParentStarStmts(StmtIndex stmt);

    /// Selects s such that Parent(stmt, s).
    /// \return all stmt#no that fit the relationship, or an empty set if there
    /// are none.
    virtual set<StmtIndex> getChildStmts(StmtIndex stmt);

    /// Selects s such that Parent*(stmt, s).
    /// \return all stmt#no that fit the relationship, or an empty set if there
    /// are none.
    virtual set<StmtIndex> getChildStarStmts(StmtIndex stmt);

    /// Selects BOOLEAN such that Parent(stmt1, stmt2).
    virtual bool parent(StmtIndex parentStmt, StmtIndex childStmt);

    /// Selects BOOLEAN such that Parent*(stmt1, stmt2).
    virtual bool parentStar(StmtIndex parentStmt, StmtIndex childStmt);

    // Modifies ================================================================

    /// Selects p such that Modifies(p, var), where p is a Procedure.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<ProcIndex> getProcsModifyingVar(VarIndex var);

    /// Selects s such that Modifies(s, var), where s is a Statement.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<StmtIndex> getStmtsModifyingVar(VarIndex var);

    /// Selects v such that Modifies(proc, v), where v is a Variable.
    /// \return all stmt#no that fit the relationship, or an empty set there are
    /// none.
    virtual set<VarIndex> getVarsModifiedByProc(ProcIndex proc);

    /// Selects v such that Modifies(stmt, v), where v is a Variable.
    /// \return all stmt#no that fit the relationship, or an empty set there are
    /// none.
    virtual set<VarIndex> getVarsModifiedByStmt(StmtIndex stmt);

    /// Selects BOOLEAN such that Modifies(proc, var).
    virtual bool procModifies(ProcIndex proc, VarIndex var);

    /// Selects BOOLEAN such that Modifies(stmt, var).
    virtual bool stmtModifies(StmtIndex stmt, VarIndex var);

    // Uses ====================================================================

    /// Selects p such that Uses(p, var), where p is a Procedure.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<ProcIndex> getProcsUsingVar(VarIndex var);

    /// Selects s such that Uses(s, var), where s is a Statement.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<StmtIndex> getStmtsUsingVar(VarIndex var);

    /// Selects v such that Uses(proc, v), where v is a Variable.
    /// \return all stmt#no that fit the relationship, or an empty set there are
    /// none.
    virtual set<VarIndex> getVarsUsedByProc(ProcIndex proc);

    /// Selects v such that Uses(stmt, v), where v is a Variable.
    /// \return all stmt#no that fit the relationship, or an empty set there are
    /// none.
    virtual set<VarIndex> getVarsUsedByStmt(StmtIndex stmt);

    /// Selects BOOLEAN such that Uses(proc, var).
    virtual bool procUses(ProcIndex proc, VarIndex var);

    /// Selects BOOLEAN such that Uses(stmt, var).
    virtual bool stmtUses(StmtIndex stmt, VarIndex var);

private:
    typedef EntityTable<Procedure, ProcIndex> ProcedureTable;
    typedef EntityTable<Variable, VarIndex> VarTable;
    typedef EntityTable<ConstantValue, ConstIndex> ConstTable;

    typedef StatementTable<Statement, StmtIndex> StatementTable;

    ProcedureTable procTable;

    VarTable varTable;
    ConstTable constTable;

    StatementTable statementTable;

    vector<StmtIndex> assignStatements;
    vector<StmtIndex> readStatements;
    vector<StmtIndex> printStatements;
    vector<StmtIndex> ifStatements;
    vector<StmtIndex> whileStatements;
    vector<StmtIndex> callStatements;

    FollowsTable followsTable;
    FollowsStarTable followsStarTable;
    ParentTable parentTable;
    ParentStarTable parentStarTable;
    ModifiesTable modifiesTable;
    UsesTable usesTable;
};
