#include "pkb/PKB.h"

using namespace std;

Iterator<VarIndex> PKB::getAllVars() { return varTable.getIndices(); }

Iterator<ConstIndex> PKB::getAllConsts() { return constTable.getIndices(); }

Iterator<ProcIndex> PKB::getAllProcs() { return procTable.getIndices(); }

Iterator<StmtIndex> PKB::getAllStmts() { return statementTable.getIndices(); }

VarIndex PKB::insertVar(Variable *variable) {
    return varTable.insert(variable);
}

ConstIndex PKB::insertConst(Const *constant) {
    return constTable.insert(constant);
}

ProcIndex PKB::insertProc(Procedure *procedure) {
    return procTable.insert(procedure);
}

StmtIndex PKB::insertStmt(Statement *statement) {
    return statementTable.insert(statement);
}

// stmt2 follows stmt1. This also adds to FollowStarTable
bool PKB::insertFollows(StmtIndex stmt1, StmtIndex stmt2) {
    followsStarTable.insertFollowsStar(stmt1, stmt2);
    return followsTable.insertFollows(stmt1, stmt2);
}

StmtIndex PKB::getFollowingStmt(StmtIndex stmt) {
    return followsTable.getFollowingStmt(stmt);
}

StmtIndex PKB::getPrecedingStmt(StmtIndex stmt) {
    return followsTable.getPrecedingStmt(stmt);
}

bool PKB::follows(StmtIndex stmt1, StmtIndex stmt2) {
    return followsTable.follows(stmt1, stmt2);
}

set<StmtIndex> PKB::getFollowingStarStmts(StmtIndex stmt) {
    return followsStarTable.getFollowingStarStmts(stmt);
}

set<StmtIndex> PKB::getPrecedingStarStmts(StmtIndex stmt) {
    return followsStarTable.getPrecedingStarStmts(stmt);
}

bool PKB::followsStar(StmtIndex stmt1, StmtIndex stmt2) {
    return followsStarTable.followsStar(stmt1, stmt2);
}

bool PKB::insertParent(StmtIndex stmt1, StmtIndex stmt2) {
    parentStarTable.insertParentStar(stmt1, stmt2);
    return parentStar(stmt1, stmt2);
}

StmtIndex PKB::getParentStmt(StmtIndex stmt) {
    return parentTable.getParentStmt(stmt);
}

StmtIndex PKB::getChildStmt(StmtIndex stmt) {
    return parentTable.getChildStmt(stmt);
}

bool PKB::parent(StmtIndex stmt1, StmtIndex stmt2) {
    return parentTable.parent(stmt1, stmt2);
}

set<StmtIndex> PKB::getParentStarStmts(StmtIndex stmt) {
    return parentStarTable.getParentStarStmts(stmt);
}

set<StmtIndex> PKB::getChildStarStmts(StmtIndex stmt) {
    return parentStarTable.getChildStarStmts(stmt);
}

bool PKB::parentStar(StmtIndex stmt1, StmtIndex stmt2) {
    return parentStarTable.parentStar(stmt1, stmt2);
}

bool PKB::insertStmtModifyingVar(StmtIndex stmt, VarIndex var) {
    return modifiesTable.insertStmtModifyingVar(stmt, var);
}

bool PKB::insertProcModifyingVar(ProcIndex proc, VarIndex var) {
    return modifiesTable.insertProcModifyingVar(proc, var);
}
set<VarIndex> PKB::getVarsModifiedByStmt(StmtIndex stmt) {
    return modifiesTable.getVarsModifiedByStmt(stmt);
}

set<StmtIndex> PKB::getStmtsModifyingVar(VarIndex var) {
    return modifiesTable.getStmtsModifyingVar(var);
}

set<VarIndex> PKB::getVarsModifiedByProc(ProcIndex proc) {
    return modifiesTable.getVarsModifiedByProc(proc);
}

set<ProcIndex> PKB::getProcsModifyingVar(VarIndex var) {
    return modifiesTable.getProcsModifyingVar(var);
}

bool PKB::stmtModifies(StmtIndex stmt, VarIndex var) {
    return modifiesTable.stmtModifies(stmt, var);
}

bool PKB::procModifies(ProcIndex proc, VarIndex var) {
    return modifiesTable.procModifies(proc, var);
}

bool PKB::insertStmtUsingVar(StmtIndex stmt, VarIndex var) {
    return usesTable.insertStmtUsingVar(stmt, var);
}

bool PKB::insertProcUsingVar(StmtIndex stmt, VarIndex var) {
    return usesTable.insertProcUsingVar(stmt, var);
}
set<VarIndex> PKB::getVarsUsedByStmt(StmtIndex stmt) {
    return usesTable.getVarsUsedByStmt(stmt);
}

set<StmtIndex> PKB::getStmtsUsingVar(VarIndex var) {
    return usesTable.getStmtsUsingVar(var);
}

set<VarIndex> PKB::getVarsUsedByProc(ProcIndex proc) {
    return usesTable.getVarsUsedByProc(proc);
}

set<ProcIndex> PKB::getProcsUsingVar(VarIndex var) {
    return usesTable.getProcsUsingVar(var);
}

bool PKB::stmtUses(StmtIndex stmt, VarIndex var) {
    return usesTable.stmtUses(stmt, var);
}

bool PKB::procUses(ProcIndex proc, VarIndex var) {
    return usesTable.procUses(proc, var);
}
