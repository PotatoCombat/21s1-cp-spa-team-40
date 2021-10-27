#pragma once

#include "ConditionTable.h"
#include "EntityTable.h"
#include "Iterator.h"
#include "PatternTable.h"
#include "RelationshipTable.h"
#include "StatementTable.h"
#include "common/model/ConstantValue.h"
#include "common/model/Procedure.h"
#include "common/model/Statement.h"
#include "common/model/Variable.h"

using namespace std;

class PKB {
public:
    /**
     * Returns the procedure with the given procedure name.
     * @param procName name of the procedure
     */
    virtual Procedure *getProcByName(ProcName procName);

    /**
     * Returns the variable with the given variable name.
     * @param varName name of the variable
     */
    virtual Variable *getVarByName(VarName varName);

    /**
     * Returns the constant value with the given constant name \param .
     * @param constName name of the constant value
     */
    virtual ConstantValue *getConstByName(ConstName constName);

    /**
     * Returns the statement with the given statement index.
     * @param stmtIndex statement index of the statement.
     */
    virtual Statement *getStmtByIndex(StmtIndex stmtIndex);

    // =========================================================================
    // Source Processor
    // =========================================================================

    /**
     * @param procedure procedure to insert into the PKB.
     */
    virtual void insertProc(Procedure *procedure);

    /**
     * @param variable variable to insert into the PKB.
     */
    virtual void insertVar(Variable *variable);

    /**
     * @param constant constant value to insert into the PKB.
     */
    virtual void insertConst(ConstantValue *constant);

    /**
     * @param statement statement to insert into the PKB.
     */
    virtual StmtIndex insertStmt(Statement *statement);

    /**
     * Stores the relationship Follows(stmt1, stmt2),
     * including transitive (*) relationships.
     * @param preceding stmt1.
     * @param following stmt2.
     */
    virtual void insertFollows(Statement *preceding, Statement *following);

    /**
     * Stores the relationship Parent(stmt1, stmt2),
     * including transitive (*) relationships.
     * @param parent stmt1.
     * @param child stmt2.
     */
    virtual void insertParent(Statement *parent, Statement *child);

    /**
     * Stores the relationship Modifies(proc, var).
     * @param proc proc.
     * @param var var.
     */
    virtual void insertProcModifyingVar(Procedure *proc, Variable *var);

    /**
     * Stores the relationship Modifies(stmt, var).
     * @param stmt stmt.
     * @param var var.
     */
    virtual void insertStmtModifyingVar(Statement *stmt, Variable *var);

    /**
     * Stores the relationship Uses(proc, var).
     * @param proc proc.
     * @param var var.
     */
    virtual void insertProcUsingVar(Procedure *proc, Variable *var);

    /**
     * Stores the relationship Uses(stmt, var).
     * @param stmt stmt.
     * @param var var.
     */
    virtual void insertStmtUsingVar(Statement *stmt, Variable *var);

    /**
     * Stores the relationship Calls(proc1, proc2),
     * including transitive (*) relationships.
     * @param proc where proc1 == proc.procName.
     * @param called proc2.
     */
    virtual void insertCalls(Procedure *proc, const ProcName &called);

    /**
     * Stores the relationship Next(stmt1, stmt2).
     * @param previous stmt1.
     * @param next stmt2.
     */
    virtual void insertNext(Statement *previous, Statement *next);

    /**
     * Stores the relationship NextBip(stmt1, stmt2).
     * @param previous stmt1.
     * @param next stmt2.
     */
    virtual void insertNextBip(Statement *previous, Statement *next);

    /**
     * Stores the pattern a(var, exprList), where a is an ASSIGN statement.
     * @param stmt ASSIGN statement a.
     */
    virtual void insertPatternAssign(Statement *stmt);

    /**
     * Stores the pattern if(var, exprList), where if is an IF statement.
     * @param stmt IF statement if.
     */
    virtual void insertIfPattern(Statement *stmt);

    /**
     * Stores the pattern while(var, _, _), where while is a WHILE statement.
     * @param stmt WHILE statement while.
     */
    virtual void insertWhilePattern(Statement *stmt);

    // =========================================================================
    // Query Processor
    // =========================================================================

    /**
     * Returns all procedure names in the program.
     */
    virtual Iterator<ProcName> getAllProcs();

    /**
     * Returns all variable names in the program.
     */
    virtual Iterator<VarName> getAllVars();

    /**
     * Returns all constant values in the program.
     */
    virtual Iterator<ConstName> getAllConsts();

    /**
     * Returns all statement indices in the program.
     */
    virtual Iterator<StmtIndex> getAllStmts();

    /**
     * Returns all statement indices of a given statement type in the program.
     * @param type statement type to find.
     */
    virtual Iterator<StmtIndex> getAllStmts(StatementType type);

    /**
     * Returns the statement type of the given statement index in the program.
     * @param stmt statement index to find.
     */
    virtual StatementType getStmtType(StmtIndex stmt);

    /**
     * Returns the variable used in the given PRINT statement.
     * @param printStmt statement index of a PRINT statement.
     */
    virtual VarName getPrintVariable(StmtIndex printStmt);

    /**
     * Returns the variable used in the given READ statement.
     * @param readStmt statement index of a READ statement.
     */
    virtual VarName getReadVariable(StmtIndex readStmt);

    /**
     * Returns the procedure used in the given CALL statement.
     * @param callStmt statement index of a CALL statement.
     */
    virtual ProcName getCallProcedure(StmtIndex callStmt);

    // Follows =================================================================

    /**
     * Selects s such that Follows(stmt, s).
     * @return stmt#no that fits the relationship,
     *             or InvalidIndex if there is none.
     */
    virtual StmtIndex getFollowingStmt(const StmtIndex &stmt);

    /**
     * Selects s such that Follows*(stmt, s).
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getFollowingStarStmts(const StmtIndex &stmt);

    /**
     * Selects s such that Follows(s, stmt).
     * @return stmt#no that fits the relationship,
     *             or InvalidIndex if there is none.
     */
    virtual StmtIndex getPrecedingStmt(const StmtIndex &stmt);

    /**
     * Selects s such that Follows*(s, stmt).
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getPrecedingStarStmts(const StmtIndex &stmt);

    /**
     * Selects BOOLEAN such that Follows(stmt1, stmt2).
     * @param preceding stmt1.
     * @param following stmt2.
     */
    virtual bool follows(const StmtIndex &preceding, const StmtIndex &following);

    /**
     * Selects BOOLEAN such that Follows*(stmt1, stmt2).
     * @param preceding stmt1.
     * @param following stmt2.
     */
    virtual bool followsStar(const StmtIndex &preceding, const StmtIndex &following);

    // Parent ==================================================================

    /**
     * Selects s such that Parent(s, stmt).
     * @return stmt#no that fits the relationship,
     *             or InvalidIndex if there is none.
     */
    virtual StmtIndex getParentStmt(const StmtIndex &stmt);

    /**
     * Selects s such that Parent*(s, stmt).
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getParentStarStmts(const StmtIndex &stmt);

    /**
     * Selects s such that Parent(stmt, s).
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getChildStmts(const StmtIndex &stmt);

    /**
     * Selects s such that Parent*(stmt, s).
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getChildStarStmts(const StmtIndex &stmt);

    /**
     * Selects BOOLEAN such that Parent(stmt1, stmt2).
     * @param parent stmt1.
     * @param child stmt2.
     */
    virtual bool parent(const StmtIndex &parent, const StmtIndex &child);

    /**
     * Selects BOOLEAN such that Parent*(stmt1, stmt2).
     * @param parent stmt1.
     * @param child stmt2.
     */
    virtual bool parentStar(const StmtIndex &parent, const StmtIndex &child);

    // Modifies ================================================================

    /**
     * Selects p such that Modifies(p, var), where p is a procedure.
     * @return all procedure names that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<ProcName> getProcsModifyingVar(const VarName &var);

    /**
     * Selects s such that Modifies(s, var), where s is a statement.
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getStmtsModifyingVar(const VarName &var);

    /**
     * Selects v such that Modifies(proc, v), where v is a variable.
     * @return all variable names that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<VarName> getVarsModifiedByProc(const ProcName &proc);

    /**
     * Selects v such that Modifies(stmt, v), where v is a variable.
     * @return all variable names that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<VarName> getVarsModifiedByStmt(const StmtIndex &stmt);

    /**
     * Selects BOOLEAN such that Modifies(proc, var).
     * @param proc proc.
     * @param var var.
     */
    virtual bool procModifies(const ProcName &proc, const VarName &var);

    /**
     * Selects BOOLEAN such that Modifies(stmt, var).
     * @param stmt stmt.
     * @param var var.
     */
    virtual bool stmtModifies(const StmtIndex &stmt, const VarName &var);

    // Uses ====================================================================

    /**
     * Selects p such that Uses(p, var), where p is a procedure.
     * @return all procedure names that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<ProcName> getProcsUsingVar(const VarName &var);

    /**
     * Selects s such that Uses(s, var), where s is a statement.
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getStmtsUsingVar(const VarName &var);

    /**
     * Selects v such that Uses(proc, v), where v is a variable.
     * @return all variable names that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<VarName> getVarsUsedByProc(const ProcName &proc);

    /**
     * Selects v such that Uses(stmt, v), where v is a variable.
     * @return all variable names that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<VarName> getVarsUsedByStmt(const StmtIndex &stmt);

    /**
     * Selects BOOLEAN such that Uses(proc, var).
     * @param proc proc.
     * @param var var.
     */
    virtual bool procUses(const ProcName &proc, const VarName &var);

    /**
     * Selects BOOLEAN such that Uses(stmt, var).
     * @param stmt stmt.
     * @param var var.
     */
    virtual bool stmtUses(const StmtIndex &stmt, const VarName &var);

    // Calls ==================================================================

    /**
     * Selects p such that Calls(caller, p).
     * @return all procedure names that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<ProcName> getCalledProcs(const ProcName &caller);

    /**
     * Selects p such that Calls*(caller, p).
     * @return all procedure names that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<ProcName> getCalledStarProcs(const ProcName &caller);

    /**
     * Selects p such that Calls(p, called).
     * @return all procedure names that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<ProcName> getCallerProcs(const ProcName &called);

    /**
     * Selects p such that Calls*(p, called).
     * @return all procedure names that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<ProcName> getCallerStarProcs(const ProcName &called);

    /**
     * Selects BOOLEAN such that Calls(p1, p2).
     * @param caller p1.
     * @param called p2.
     */
    virtual bool calls(const ProcName &caller, const ProcName &called);

    /**
     * Selects BOOLEAN such that Calls*(p1, p2).
     * @param caller p1.
     * @param called p2.
     */
    virtual bool callsStar(const ProcName &caller, const ProcName &called);

    // Next ==================================================================

    /**
     * Selects s such that Next(line, s).
     * @return all program line numbers that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getNextLines(const ProgLineIndex &line);

    /**
     * Selects s such that Next(s, line).
     * @return all program line numbers that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getPreviousLines(const ProgLineIndex &line);

    /**
     * Selects BOOLEAN such that Next(line1, line2).
     * @param previous line1.
     * @param next line2.
     */
    virtual bool next(const ProgLineIndex &previous, const ProgLineIndex &next);

    // NextBip ===============================================================

    /**
     * Selects s such that NextBip(line, s).
     * @return all program line numbers that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getNextBipLines(const ProgLineIndex &line);

    /**
     * Selects s such that NextBip(s, line).
     * @return all program line numbers that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getPreviousBipLines(const ProgLineIndex &line);

    /**
     * Selects BOOLEAN such that NextBip(line1, line2).
     * @param previous line1.
     * @param next line2.
     */
    virtual bool nextBip(const ProgLineIndex &previous, const ProgLineIndex &next);

    // Pattern =================================================================

    /**
     * Selects a such that a(var, pattern), where a is an ASSIGN statement.
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getPartialAssignPatternStmts(VarName var,
                                                        ExpressionList pattern);

    /**
     * Selects a such that a(var, pattern), where a is an ASSIGN statement,
     * and the pattern requires an exact match.
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getExactAssignPatternStmts(VarName var,
                                                      ExpressionList pattern);

    /**
     * Selects if such that if(var, _, _), where if is an IF statement.
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getIfPatternStmts(VarName var);

    /**
     * Selects while such that while(var, _), where while is a WHILE statement.
     * @return all stmt#no that fit the relationship,
     *             or an empty set if there are none.
     */
    virtual set<StmtIndex> getWhilePatternStmts(VarName var);

    /**
     * Selects BOOLEAN such that a(var, pattern), where a is an ASSIGN
     * statement.
     * @param stmt statement index of the ASSIGN statement.
     * @param var var.
     * @param pattern expression list (tokenized infix expression).
     */
    virtual bool partialAssignPattern(StmtIndex stmt, VarName var,
                                      ExpressionList pattern);

    /**
     * Selects BOOLEAN such that a(var, pattern), where a is an ASSIGN
     * statement, and the pattern requires an exact match.
     * @param stmt statement index of the ASSIGN statement.
     * @param var var.
     * @param pattern expression list (tokenized infix expression).
     */
    virtual bool exactAssignPattern(StmtIndex stmt, VarName var,
                                    ExpressionList pattern);

    /// Selects BOOLEAN such that if(var, _, _), where if is an IF statement.

    /**
     * Selects BOOLEAN such that if(var, _, _), where if is an IF statement.
     * @param stmt statement index of the IF statement.
     * @param var var.
     */
    virtual bool ifPattern(StmtIndex stmt, VarName var);

    /**
     * Selects BOOLEAN such that while(var, _), where while is a WHILE
     * statement.
     * @param stmt statement index of the WHILE statement.
     * @param var var.
     */
    virtual bool whilePattern(StmtIndex stmtIndex, VarName var);

private:
    EntityTable<Procedure, ProcName> procTable;
    EntityTable<Variable, VarName> varTable;
    EntityTable<ConstantValue, ConstName> constTable;
    StatementTable statementTable;

    RelationshipTable<StmtIndex, StmtIndex> followsTable;
    RelationshipTable<StmtIndex, StmtIndex> followsStarTable;
    RelationshipTable<StmtIndex, StmtIndex> parentTable;
    RelationshipTable<StmtIndex, StmtIndex> parentStarTable;
    RelationshipTable<ProcName, VarName> modifiesProcTable;
    RelationshipTable<StmtIndex, VarName> modifiesStmtTable;
    RelationshipTable<ProcName, VarName> usesProcTable;
    RelationshipTable<StmtIndex, VarName> usesStmtTable;
    RelationshipTable<ProcName, ProcName> callsTable;
    RelationshipTable<ProcName, ProcName> callsStarTable;
    RelationshipTable<ProgLineIndex, ProgLineIndex> nextTable;
    RelationshipTable<ProgLineIndex, ProgLineIndex> nextBipTable;
    PatternTable patternTable;
    ConditionTable conditionTable;
};
