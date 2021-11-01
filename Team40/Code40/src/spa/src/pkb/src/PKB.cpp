#include "pkb/PKB.h"

#include <utility>

using namespace std;

Procedure *PKB::getProcByName(ProcName procName) {
    return procTable.getEntity(move(procName));
}

Variable *PKB::getVarByName(VarName varName) {
    return varTable.getEntity(move(varName));
}

ConstantValue *PKB::getConstByName(ConstName constName) {
    return constTable.getEntity(move(constName));
}

Statement *PKB::getStmtByIndex(StmtIndex stmtIndex) {
    return statementTable.getStmt(stmtIndex);
}

// =============================================================================
// Source Processor
// =============================================================================

void PKB::insertProc(Procedure *procedure) {
    return procTable.insert(procedure);
}

void PKB::insertVar(Variable *variable) { return varTable.insert(variable); }

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

void PKB::insertCalls(Procedure *proc, ProcName called) {
    callsTable.insertCalls(proc, called);
    callsStarTable.insertCallsStar(proc, called);
}

void PKB::insertNext(Statement *previousStmt, Statement *nextStmt) {
    nextTable.insertNext(previousStmt, nextStmt);
}

void PKB::insertNextBip(Statement *previousStmt, Statement *nextStmt) {
    nextBipTable.insertNextBip(previousStmt, nextStmt);
}

void PKB::insertPatternAssign(Statement *stmt) {
    patternTable.insertAssignPattern(stmt);
}

void PKB::insertIfPattern(Statement *stmt) {
    conditionTable.insertIfPattern(stmt);
}

void PKB::insertWhilePattern(Statement *stmt) {
    conditionTable.insertWhilePattern(stmt);
}

void PKB::insertBranchIn(Statement *fromStmt, Statement *toStmt) {
    branchInTable.insertBranchIn(fromStmt, toStmt);
}

void PKB::insertBranchBack(Statement *fromStmt, Statement *toStmt) {
    branchBackTable.insertBranchBack(fromStmt, toStmt);
}

// =============================================================================
// Query Processor
// =============================================================================

Iterator<ProcName> PKB::getAllProcs() { return procTable.getNames(); }

Iterator<VarName> PKB::getAllVars() { return varTable.getNames(); }

Iterator<ConstName> PKB::getAllConsts() { return constTable.getNames(); }

Iterator<StmtIndex> PKB::getAllStmts() { return statementTable.getIndices(); }

Iterator<StmtIndex> PKB::getAllStmts(StatementType type) {
    return statementTable.getIndices(type);
}

StatementType PKB::getStmtType(StmtIndex stmt) {
    return statementTable.getStmtType(stmt);
}

VarName PKB::getPrintVariable(StmtIndex printStmt) {
    auto stmt = statementTable.getStmt(printStmt);
    if (stmt->getStatementType() != StatementType::PRINT) {
        throw runtime_error(
            "This statement number does not refer to a print statement.");
    }
    return stmt->getVariable()->getName();
}

VarName PKB::getReadVariable(StmtIndex readStmt) {
    auto stmt = statementTable.getStmt(readStmt);
    if (stmt->getStatementType() != StatementType::READ) {
        throw runtime_error(
            "This statement number does not refer to a read statement.");
    }
    return stmt->getVariable()->getName();
}

ProcName PKB::getCallProcedure(StmtIndex callStmt) {
    auto stmt = statementTable.getStmt(callStmt);
    if (stmt->getStatementType() != StatementType::CALL) {
        throw runtime_error(
            "This statement number does not refer to a call statement.");
    }
    return stmt->getProcName();
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

// Calls =======================================================================

set<ProcName> PKB::getCalledProcs(ProcName caller) {
    return callsTable.getCalledProcs(caller);
}

set<ProcName> PKB::getCalledStarProcs(ProcName caller) {
    return callsStarTable.getCalledStarProcs(caller);
}

set<ProcName> PKB::getCallerProcs(ProcName called) {
    return callsTable.getCallerProcs(called);
}

set<ProcName> PKB::getCallerStarProcs(ProcName called) {
    return callsStarTable.getCallerStarProcs(called);
}

bool PKB::calls(ProcName caller, ProcName called) {
    return callsTable.calls(caller, called);
}

bool PKB::callsStar(ProcName caller, ProcName called) {
    return callsStarTable.callsStar(caller, called);
}

// Next ========================================================================

set<StmtIndex> PKB::getNextLines(ProgLineIndex line) {
    return nextTable.getNextLines(line);
}

set<StmtIndex> PKB::getPreviousLines(ProgLineIndex line) {
    return nextTable.getPreviousLines(line);
}

bool PKB::next(ProgLineIndex previousLine, ProgLineIndex nextLine) {
    return nextTable.next(previousLine, nextLine);
}

// NextBip =====================================================================

set<StmtIndex> PKB::getNextBipLines(ProgLineIndex line) {
    return nextBipTable.getNextBipLines(line);
}

set<StmtIndex> PKB::getPreviousBipLines(ProgLineIndex line) {
    return nextBipTable.getPreviousBipLines(line);
}

bool PKB::nextBip(ProgLineIndex previousLine, ProgLineIndex nextLine) {
    return nextBipTable.nextBip(previousLine, nextLine);
}

StmtIndex PKB::getBranchInToLine(ProgLineIndex line) {
    return branchInTable.getBranchInToStmt(line);
}

set<StmtIndex> PKB::getBranchInFromLines(ProgLineIndex line) {
    return branchInTable.getBranchInFromStmts(line);
}

bool PKB::branchIn(ProgLineIndex fromLine, ProgLineIndex toLine) {
    return branchInTable.branchIn(fromLine, toLine);
}

set<StmtIndex> PKB::getBranchBackToLines(ProgLineIndex line) {
    return branchBackTable.getBranchBackToStmts(line);
}

set<StmtIndex> PKB::getBranchBackFromLines(ProgLineIndex line) {
    return branchBackTable.getBranchBackFromStmts(line);
}

bool PKB::branchBack(ProgLineIndex fromLine, ProgLineIndex toLine) {
    return branchBackTable.branchBack(fromLine, toLine);
}

// Pattern =====================================================================

set<StmtIndex> PKB::getPartialAssignPatternStmts(VarName var,
                                                 ExpressionList pattern) {
    return patternTable.getPartialAssignPatternStmts(var, pattern);
}

set<StmtIndex> PKB::getExactAssignPatternStmts(VarName var,
                                               ExpressionList pattern) {
    return patternTable.getExactAssignPatternStmts(var, pattern);
}

set<StmtIndex> PKB::getIfPatternStmts(VarName var) {
    return conditionTable.getIfPatternStmts(var);
}

set<StmtIndex> PKB::getWhilePatternStmts(VarName var) {
    return conditionTable.getWhilePatternStmts(var);
}

bool PKB::partialAssignPattern(StmtIndex stmtIndex, VarName var,
                               ExpressionList pattern) {
    return patternTable.partialAssignPattern(stmtIndex, var, pattern);
}

bool PKB::exactAssignPattern(StmtIndex stmtIndex, VarName var,
                             ExpressionList pattern) {
    return patternTable.exactAssignPattern(stmtIndex, var, pattern);
}

bool PKB::ifPattern(StmtIndex stmtIndex, VarName var) {
    return conditionTable.ifPattern(stmtIndex, var);
}

bool PKB::whilePattern(StmtIndex stmtIndex, VarName var) {
    return conditionTable.whilePattern(stmtIndex, var);
}
