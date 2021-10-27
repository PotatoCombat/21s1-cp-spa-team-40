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

void PKB::insertFollows(Statement *preceding, Statement *following) {
    followsTable.insertRelationship(preceding->getIndex(), following->getIndex());
    followsStarTable.insertRelationship(preceding->getIndex(), following->getIndex());
    followsStarTable.updateTransitivity(preceding->getIndex(), following->getIndex());
}

void PKB::insertParent(Statement *parent, Statement *child) {
    parentTable.insertRelationship(parent->getIndex(), child->getIndex());
    parentStarTable.insertRelationship(parent->getIndex(), child->getIndex());
    parentStarTable.updateTransitivity(parent->getIndex(), child->getIndex());
}

void PKB::insertProcModifyingVar(Procedure *proc, Variable *var) {
    modifiesProcTable.insertRelationship(proc->getName(), var->getName());
}

void PKB::insertStmtModifyingVar(Statement *stmt, Variable *var) {
    modifiesStmtTable.insertRelationship(stmt->getIndex(), var->getName());
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

StmtIndex PKB::getFollowingStmt(const StmtIndex &stmt) {
    return followsTable.getFirstRHSRelationship(stmt, InvalidIndex);
}

set<StmtIndex> PKB::getFollowingStarStmts(const StmtIndex &stmt) {
    return followsStarTable.getRHSRelationships(stmt);
}

StmtIndex PKB::getPrecedingStmt(const StmtIndex &stmt) {
    return followsTable.getFirstLHSRelationship(stmt, InvalidIndex);
}

set<StmtIndex> PKB::getPrecedingStarStmts(const StmtIndex &stmt) {
    return followsStarTable.getLHSRelationships(stmt);
}

bool PKB::follows(const StmtIndex &preceding, const StmtIndex &following) {
    return followsTable.isRelationship(preceding, following);
}

bool PKB::followsStar(const StmtIndex &preceding, const StmtIndex &following) {
    return followsStarTable.isRelationship(preceding, following);
}

// Parent ======================================================================

StmtIndex PKB::getParentStmt(const StmtIndex &stmt) {
    return parentTable.getFirstLHSRelationship(stmt, InvalidIndex);
}

set<StmtIndex> PKB::getParentStarStmts(const StmtIndex &stmt) {
    return parentStarTable.getLHSRelationships(stmt);
}

set<StmtIndex> PKB::getChildStmts(const StmtIndex &stmt) {
    return parentTable.getRHSRelationships(stmt);
}

set<StmtIndex> PKB::getChildStarStmts(const StmtIndex &stmt) {
    return parentStarTable.getRHSRelationships(stmt);
}

bool PKB::parent(const StmtIndex &parent, const StmtIndex &child) {
    return parentTable.isRelationship(parent, child);
}

bool PKB::parentStar(const StmtIndex &parent, const StmtIndex &child) {
    return parentStarTable.isRelationship(parent, child);
}

// Modifies ====================================================================

set<ProcName> PKB::getProcsModifyingVar(const VarName &var) {
    return modifiesProcTable.getLHSRelationships(var);
}

set<StmtIndex> PKB::getStmtsModifyingVar(const VarName &var) {
    return modifiesStmtTable.getLHSRelationships(var);
}

set<VarName> PKB::getVarsModifiedByProc(const ProcName &proc) {
    return modifiesProcTable.getRHSRelationships(proc);
}

set<VarName> PKB::getVarsModifiedByStmt(const StmtIndex &stmt) {
    return modifiesStmtTable.getRHSRelationships(stmt);
}

bool PKB::procModifies(const ProcName &proc, const VarName &var) {
    return modifiesProcTable.isRelationship(proc, var);
}

bool PKB::stmtModifies(const StmtIndex &stmt, const VarName &var) {
    return modifiesStmtTable.isRelationship(stmt, var);
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
