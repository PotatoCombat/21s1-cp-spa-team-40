#pragma once

#include "EntityTable.h"
#include "FollowsStarTable.h"
#include "FollowsTable.h"
#include "Iterator.h"
#include "ModifiesTable.h"
#include "ParentStarTable.h"
#include "ParentTable.h"
#include "PatternTable.h"
#include "StatementTable.h"
#include "UsesTable.h"
#include "NextTable.h"
#include "CallsTable.h"
#include "CallsStarTable.h"
#include "ConditionTable.h"
#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Statement.h"
#include "common/model/Variable.h"

using namespace std;

class PKB {
public:
    typedef EntityTable<Procedure, ProcName> ProcedureTable;
    typedef EntityTable<Variable, VarName> VarTable;
    typedef EntityTable<ConstantValue, ConstName> ConstTable;

    /// Returns the procedure with the given procedure name \param procName.
    virtual Procedure *getProcByName(ProcName procName);

    /// Returns the variable with the given variable name \param varName.
    virtual Variable *getVarByName(VarName varName);

    /// Returns the constant with the given variable name \param constName.
    virtual ConstantValue *getConstByName(ConstName constName);

    /// Returns the statement with the given statement index \param stmtIndex.
    virtual Statement *getStmtByIndex(StmtIndex stmtIndex);

    // =========================================================================
    // Source Processor
    // =========================================================================

    virtual void insertProc(Procedure *procedure);
    virtual void insertVar(Variable *variable);
    virtual void insertConst(ConstantValue *constant);

    virtual StmtIndex insertStmt(Statement *statement);

    /// Stores the relationship Follows(stmt1, stmt2), and updates *
    /// relationships.
    virtual void insertFollows(Statement *precedingStmt,
                               Statement *followingStmt);

    /// Stores the relationship Parent(stmt1, stmt2), and updates *
    /// relationships.
    virtual void insertParent(Statement *parentStmt, Statement *childStmt);

    /// Stores the relationship Modifies(proc, var).
    virtual void insertProcModifyingVar(Procedure *proc, Variable *var);

    /// Stores the relationship Modifies(stmt, var).
    virtual void insertStmtModifyingVar(Statement *stmt, Variable *var);

    /// Stores the relationship Uses(proc, var).
    virtual void insertProcUsingVar(Procedure *proc, Variable *var);

    /// Stores the relationship Uses(stmt, var).
    virtual void insertStmtUsingVar(Statement *stmt, Variable *var);

    /// Stores the relationship Calls(stmt1, stmt2), and updates *
    /// relationships.
    virtual void insertCalls(Procedure *proc, ProcName called);

    /// Stores the relationship Next(stmt1, stmt2).
    virtual void insertNext(Statement *previousStmt, Statement *nextStmt);

    /// Stores the pattern a(var, exprList), where a is an ASSIGN statement.
    virtual void insertPatternAssign(Statement *stmt);

    /// Stores the pattern if(var, _, _), where if is an IF statement.
    virtual void insertIfPattern(Statement *stmt);

    /// Stores the pattern while(var, _, _), where while is a WHILE statement.
    virtual void insertWhilePattern(Statement *stmt);

    // =========================================================================
    // Query Processor
    // =========================================================================

    virtual Iterator<ProcName> getAllProcs();
    virtual Iterator<VarName> getAllVars();
    virtual Iterator<ConstName> getAllConsts();

    virtual Iterator<StmtIndex> getAllStmts();
    virtual Iterator<StmtIndex> getAllStmts(StatementType type);

    virtual StatementType getStmtType(StmtIndex stmt);

    virtual VarName getPrintVariable(StmtIndex printStmt);
    virtual VarName getReadVariable(StmtIndex readStmt);
    virtual ProcName getCallProcedure(StmtIndex callStmt);

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
    virtual set<ProcName> getProcsModifyingVar(VarName var);

    /// Selects s such that Modifies(s, var), where s is a Statement.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<StmtIndex> getStmtsModifyingVar(VarName var);

    /// Selects v such that Modifies(proc, v), where v is a Variable.
    /// \return all stmt#no that fit the relationship, or an empty set there are
    /// none.
    virtual set<VarName> getVarsModifiedByProc(ProcName proc);

    /// Selects v such that Modifies(stmt, v), where v is a Variable.
    /// \return all stmt#no that fit the relationship, or an empty set there are
    /// none.
    virtual set<VarName> getVarsModifiedByStmt(StmtIndex stmt);

    /// Selects BOOLEAN such that Modifies(proc, var).
    virtual bool procModifies(ProcName proc, VarName var);

    /// Selects BOOLEAN such that Modifies(stmt, var).
    virtual bool stmtModifies(StmtIndex stmt, VarName var);

    // Uses ====================================================================

    /// Selects p such that Uses(p, var), where p is a Procedure.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<ProcName> getProcsUsingVar(VarName var);

    /// Selects s such that Uses(s, var), where s is a Statement.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<StmtIndex> getStmtsUsingVar(VarName var);

    /// Selects v such that Uses(proc, v), where v is a Variable.
    /// \return all stmt#no that fit the relationship, or an empty set there are
    /// none.
    virtual set<VarName> getVarsUsedByProc(ProcName proc);

    /// Selects v such that Uses(stmt, v), where v is a Variable.
    /// \return all stmt#no that fit the relationship, or an empty set there are
    /// none.
    virtual set<VarName> getVarsUsedByStmt(StmtIndex stmt);

    /// Selects BOOLEAN such that Uses(proc, var).
    virtual bool procUses(ProcName proc, VarName var);

    /// Selects BOOLEAN such that Uses(stmt, var).
    virtual bool stmtUses(StmtIndex stmt, VarName var);

    // Calls ==================================================================

    /// Selects p such that Calls(caller, p).
    /// \return p#procName that fits the relationship, or an empty set if there
    /// are none.
    virtual set<ProcName> getCalledProcs(ProcName caller);

    /// Selects p such that Calls*(caller, p).
    /// \return p#procName that fits the relationship, or an empty set if there
    /// are none.
    virtual set<ProcName> getCalledStarProcs(ProcName caller);

    /// Selects p such that Calls(p, called).
    /// \return p#procName that fits the relationship, or an empty set if there
    /// are none.
    virtual set<ProcName> getCallerProcs(ProcName called);

    /// Selects p such that Calls*(p, called).
    /// \return p#procName that fits the relationship, or an empty set if there
    /// are none.
    virtual set<ProcName> getCallerStarProcs(ProcName called);

    /// Selects BOOLEAN such that Calls(proc1, proc2).
    virtual bool calls(ProcName caller, ProcName called);

    /// Selects BOOLEAN such that Calls*(proc1, proc2).
    virtual bool callsStar(ProcName caller, ProcName called);

    // Next ==================================================================

    /// Selects s such that Next(line, s).
    /// \return all programLine#no that fits the relationship, or an empty set if there
    /// are none.
    virtual set<StmtIndex> getNextLines(ProgLineIndex line);

    /// Selects s such that Next(s, line).
    /// \return all programLine#no that fits the relationship, or an empty set if there
    /// are none.
    virtual set<StmtIndex> getPreviousLines(ProgLineIndex line);

    /// Selects BOOLEAN such that Next(line1, line2).
    virtual bool next(ProgLineIndex previousLine, ProgLineIndex nextLine);

    // Pattern =================================================================

    /// Selects a such that a(var, pattern), where a is an ASSIGN statement.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<StmtIndex> getPartialAssignPatternStmts(VarName var,
                                                        ExpressionList pattern);

    /// Selects a such that a(var, pattern), where a is an ASSIGN statement,
    /// and the pattern requires an exact match.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<StmtIndex> getExactAssignPatternStmts(VarName var,
                                                      ExpressionList pattern);

    /// Selects if such that if(var, _, _), where if is an IF statement.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<StmtIndex> getIfPatternStmts(VarName var);

    /// Selects while such that while(var, _), where while is a WHILE statement.
    /// \return stmt#no that fits the relationship, or an empty set there are
    /// none.
    virtual set<StmtIndex> getWhilePatternStmts(VarName var);

    /// Selects BOOLEAN such that a(var, pattern), where a is an ASSIGN statement.
    virtual bool partialAssignPattern(StmtIndex stmtIndex, VarName var,
                                      ExpressionList pattern);

    /// Selects BOOLEAN such that a(var, pattern), where a is an ASSIGN statement
    /// and the pattern must be an exact match.
    virtual bool exactAssignPattern(StmtIndex stmtIndex, VarName var,
                                    ExpressionList pattern);

    /// Selects BOOLEAN such that if(var, _, _), where if is an IF statement.
    virtual bool ifPattern(StmtIndex stmtIndex, VarName var);

    /// Selects BOOLEAN such that while(var, _), where while is a WHILE statement.
    virtual bool whilePattern(StmtIndex stmtIndex, VarName var);

private:
    ProcedureTable procTable;
    VarTable varTable;
    ConstTable constTable;
    StatementTable statementTable;

    FollowsTable followsTable;
    FollowsStarTable followsStarTable;
    ParentTable parentTable;
    ParentStarTable parentStarTable;
    ModifiesTable modifiesTable;
    UsesTable usesTable;
    CallsTable callsTable;
    CallsStarTable callsStarTable;
    NextTable nextTable;
    PatternTable patternTable;
    ConditionTable conditionTable;
};
