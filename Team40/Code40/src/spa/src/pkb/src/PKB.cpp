#include "pkb/PKB.h"

using namespace std;

// =============================================================================
// Source Processor
// =============================================================================

ProcIndex PKB::insertProc(Procedure *procedure) {
    return procTable.insert(procedure);
}

StmtIndex PKB::insertStmt(Statement *statement) {
    switch (statement->getStatementType()) {
    case StatementType::ASSIGN:
        assignStatements.push_back(statement->getIndex());
        break;
    case StatementType::READ:
        readStatements.push_back(statement->getIndex());
        break;
    case StatementType::PRINT:
        printStatements.push_back(statement->getIndex());
        break;
    case StatementType::IF:
        ifStatements.push_back(statement->getIndex());
        break;
    case StatementType::WHILE:
        whileStatements.push_back(statement->getIndex());
        break;
    case StatementType::CALL:
        callStatements.push_back(statement->getIndex());
        break;
    default:
        throw runtime_error("Cannot store a statement with an unknown statement type");
    }
    return statementTable.insert(statement);
}

VarIndex PKB::insertVar(Variable *variable) {
    return varTable.insert(variable);
}

ConstIndex PKB::insertConst(Const *constant) {
    return constTable.insert(constant);
}

void PKB::insertFollows(StmtIndex precedingStmt, StmtIndex followingStmt) {
    followsTable.insertFollows(precedingStmt, followingStmt);
    followsStarTable.insertFollowsStar(precedingStmt, followingStmt);
}

void PKB::insertParent(StmtIndex parentStmt, StmtIndex childStmt) {
    parentTable.insertParent(parentStmt, childStmt);
    parentStarTable.insertParentStar(parentStmt, childStmt);
}

void PKB::insertProcModifyingVar(ProcIndex proc, VarIndex var) {
    modifiesTable.insertProcModifyingVar(proc, var);
}

void PKB::insertStmtModifyingVar(StmtIndex stmt, VarIndex var) {
    modifiesTable.insertStmtModifyingVar(stmt, var);
}

void PKB::insertProcUsingVar(ProcIndex proc, VarIndex var) {
    usesTable.insertProcUsingVar(proc, var);
}

void PKB::insertStmtUsingVar(StmtIndex stmt, VarIndex var) {
    usesTable.insertStmtUsingVar(stmt, var);
}

// =============================================================================
// Query Processor
// =============================================================================

Iterator<ProcIndex> PKB::getAllProcs() { return procTable.getIndices(); }

Iterator<StmtIndex> PKB::getAllStmts() { return statementTable.getIndices(); }

Iterator<StmtIndex> PKB::getAllStmts(StatementType type) {
    switch (type) {
    case StatementType::ASSIGN:
        return {assignStatements};
    case StatementType::READ:
        return {readStatements};
    case StatementType::PRINT:
        return {printStatements};
    case StatementType::IF:
        return {ifStatements};
    case StatementType::WHILE:
        return {whileStatements};
    case StatementType::CALL:
        return {callStatements};
    default:
        throw runtime_error("Cannot fetch an iterator of statements of an unknown statement type");
    }
}

Iterator<VarIndex> PKB::getAllVars() { return varTable.getIndices(); }

Iterator<ConstIndex> PKB::getAllConsts() { return constTable.getIndices(); }

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

set<ProcIndex> PKB::getProcsModifyingVar(VarIndex var) {
    return modifiesTable.getProcsModifyingVar(var);
}

set<StmtIndex> PKB::getStmtsModifyingVar(VarIndex var) {
    return modifiesTable.getStmtsModifyingVar(var);
}

set<VarIndex> PKB::getVarsModifiedByProc(ProcIndex proc) {
    return modifiesTable.getVarsModifiedByProc(proc);
}

set<VarIndex> PKB::getVarsModifiedByStmt(StmtIndex stmt) {
    return modifiesTable.getVarsModifiedByStmt(stmt);
}

bool PKB::procModifies(ProcIndex proc, VarIndex var) {
    return modifiesTable.procModifies(proc, var);
}

bool PKB::stmtModifies(StmtIndex stmt, VarIndex var) {
    return modifiesTable.stmtModifies(stmt, var);
}

// Uses ========================================================================

set<ProcIndex> PKB::getProcsUsingVar(VarIndex var) {
    return usesTable.getProcsUsingVar(var);
}

set<StmtIndex> PKB::getStmtsUsingVar(VarIndex var) {
    return usesTable.getStmtsUsingVar(var);
}

set<VarIndex> PKB::getVarsUsedByProc(ProcIndex proc) {
    return usesTable.getVarsUsedByProc(proc);
}

set<VarIndex> PKB::getVarsUsedByStmt(StmtIndex stmt) {
    return usesTable.getVarsUsedByStmt(stmt);
}

bool PKB::procUses(ProcIndex proc, VarIndex var) {
    return usesTable.procUses(proc, var);
}

bool PKB::stmtUses(StmtIndex stmt, VarIndex var) {
    return usesTable.stmtUses(stmt, var);
}
