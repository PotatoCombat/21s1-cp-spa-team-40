#include "pkb/PKB.h"

#include <utility>

using namespace std;

Procedure* PKB::getProcByName(ProcName procName) {
    return procTable.getEntity(move(procName));
}

Variable* PKB::getVarByName(VarName varName) {
    return varTable.getEntity(move(varName));
}

Statement* PKB::getStmtByIndex(StmtIndex stmtIndex) {
    return statementTable.getStmt(stmtIndex);
}

// =============================================================================
// Source Processor
// =============================================================================

void PKB::insertProc(Procedure *procedure) {
    return procTable.insert(procedure);
}

void PKB::insertVar(Variable *variable) {
    return varTable.insert(variable);
}

void PKB::insertConst(ConstantValue *constant) {
    return constTable.insert(constant);
}

StmtIndex PKB::insertStmt(Statement *statement) {
    return statementTable.insert(statement);
}

void PKB::insertFollows(Statement *precedingStmt, Statement *followingStmt) {
    followsTable.insertFollows(precedingStmt, followingStmt);
    followsStarTable.insertFollowsStar(precedingStmt, followingStmt);
}

void PKB::insertParent(Statement *parentStmt, Statement *childStmt) {
    parentTable.insertParent(parentStmt, childStmt);
    parentStarTable.insertParentStar(parentStmt, childStmt);
}

void PKB::insertProcModifyingVar(Procedure *proc, Variable *var) {
    modifiesTable.insertProcModifyingVar(proc, var);
}

void PKB::insertStmtModifyingVar(Statement *stmt, Variable *var) {
    modifiesTable.insertStmtModifyingVar(stmt, var);
}

void PKB::insertProcUsingVar(Procedure *proc, Variable *var) {
    usesTable.insertProcUsingVar(proc, var);
}

void PKB::insertStmtUsingVar(Statement *stmt, Variable *var) {
    usesTable.insertStmtUsingVar(stmt, var);
}

void PKB::insertPatternAssign(AssignStatement *stmt) {
    patternTable.insertPatternAssign(stmt);
}

// =============================================================================
// Query Processor
// =============================================================================

Iterator<ProcName> PKB::getAllProcs() { return procTable.getNames(); }

Iterator<VarName> PKB::getAllVars() { return varTable.getNames(); }

Iterator<int> PKB::getAllConsts() { return constTable.getValues(); }

Iterator<StmtIndex> PKB::getAllStmts() { return statementTable.getIndices(); }

Iterator<StmtIndex> PKB::getAllStmts(StatementType type) {
    return statementTable.getIndices(type);
}

StatementType PKB::getStmtType(StmtIndex stmt) {
    return statementTable.getStmtType(stmt);
}

// Follows =====================================================================

StmtIndex PKB::getFollowingStmt(StmtIndex stmt) {
    return followsTable.getFollowingStmt(stmt);
}

set<StmtIndex> PKB::getFollowingStarStmts(StmtIndex stmt) {
    return followsStarTable.getFollowingStarStmts(stmt);
}

StmtIndex PKB::getPrecedingStmt(StmtIndex stmt) {
    return followsTable.getPrecedingStmt(stmt);
}

set<StmtIndex> PKB::getPrecedingStarStmts(StmtIndex stmt) {
    return followsStarTable.getPrecedingStarStmts(stmt);
}

bool PKB::follows(StmtIndex precedingStmt, StmtIndex followingStmt) {
    return followsTable.follows(precedingStmt, followingStmt);
}

bool PKB::followsStar(StmtIndex precedingStmt, StmtIndex followingStmt) {
    return followsStarTable.followsStar(precedingStmt, followingStmt);
}

// Parent ======================================================================

StmtIndex PKB::getParentStmt(StmtIndex stmt) {
    return parentTable.getParentStmt(stmt);
}

set<StmtIndex> PKB::getParentStarStmts(StmtIndex stmt) {
    return parentStarTable.getParentStarStmts(stmt);
}

set<StmtIndex> PKB::getChildStmts(StmtIndex stmt) {
    return parentTable.getChildStmts(stmt);
}

set<StmtIndex> PKB::getChildStarStmts(StmtIndex stmt) {
    return parentStarTable.getChildStarStmts(stmt);
}

bool PKB::parent(StmtIndex parentStmt, StmtIndex childStmt) {
    return parentTable.parent(parentStmt, childStmt);
}

bool PKB::parentStar(StmtIndex parentStmt, StmtIndex childStmt) {
    return parentStarTable.parentStar(parentStmt, childStmt);
}

// Modifies ====================================================================

set<ProcName> PKB::getProcsModifyingVar(VarName var) {
    return modifiesTable.getProcsModifyingVar(var);
}

set<StmtIndex> PKB::getStmtsModifyingVar(VarName var) {
    return modifiesTable.getStmtsModifyingVar(var);
}

set<VarName> PKB::getVarsModifiedByProc(ProcName proc) {
    return modifiesTable.getVarsModifiedByProc(proc);
}

set<VarName> PKB::getVarsModifiedByStmt(StmtIndex stmt) {
    return modifiesTable.getVarsModifiedByStmt(stmt);
}

bool PKB::procModifies(ProcName proc, VarName var) {
    return modifiesTable.procModifies(proc, var);
}

bool PKB::stmtModifies(StmtIndex stmt, VarName var) {
    return modifiesTable.stmtModifies(stmt, var);
}

// Uses ========================================================================

set<ProcName> PKB::getProcsUsingVar(VarName var) {
    return usesTable.getProcsUsingVar(var);
}

set<StmtIndex> PKB::getStmtsUsingVar(VarName var) {
    return usesTable.getStmtsUsingVar(var);
}

set<VarName> PKB::getVarsUsedByProc(ProcName proc) {
    return usesTable.getVarsUsedByProc(proc);
}

set<VarName> PKB::getVarsUsedByStmt(StmtIndex stmt) {
    return usesTable.getVarsUsedByStmt(stmt);
}

bool PKB::procUses(ProcName proc, VarName var) {
    return usesTable.procUses(proc, var);
}

bool PKB::stmtUses(StmtIndex stmt, VarName var) {
    return usesTable.stmtUses(stmt, var);
}

// Pattern =====================================================================

set<StmtIndex> PKB::getAssignsMatchingPattern(VarName var, Pattern pattern) {
    return patternTable.getAssignsMatchingPattern(var, pattern);
}

set<StmtIndex> PKB::getAssignsMatchingExactPattern(VarName var, Pattern pattern) {
    return patternTable.getAssignsMatchingExactPattern(var, pattern);
}

bool PKB::assignMatchesPattern(StmtIndex stmtIndex, VarName var, Pattern pattern) {
    return patternTable.assignMatchesPattern(stmtIndex, var, pattern);
}

bool PKB::assignMatchesExactPattern(StmtIndex stmtIndex, VarName var, Pattern pattern) {
    return patternTable.assignMatchesExactPattern(stmtIndex, var, pattern);
}
