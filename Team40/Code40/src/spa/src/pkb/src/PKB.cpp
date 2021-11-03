#include "pkb/PKB.h"

using namespace std;

Procedure *PKB::getProcByName(const ProcName &procName) {
    return procTable.getEntity(procName);
}

Variable *PKB::getVarByName(const VarName &varName) {
    return varTable.getEntity(varName);
}

ConstantValue *PKB::getConstByName(const ConstName &constName) {
    return constTable.getEntity(constName);
}

Statement *PKB::getStmtByIndex(const StmtIndex &stmtIndex) {
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

void PKB::insertStmt(Statement *statement) {
    return statementTable.insert(statement);
}

void PKB::insertFollows(Statement *preceding, Statement *following) {
    followsTable.insertRelationship(preceding->getId(), following->getId());
    followsStarTable.insertRelationship(preceding->getId(), following->getId());
    followsStarTable.updateTransitivity(preceding->getId(), following->getId());
}

void PKB::insertParent(Statement *parent, Statement *child) {
    parentTable.insertRelationship(parent->getId(), child->getId());
    parentStarTable.insertRelationship(parent->getId(), child->getId());
    parentStarTable.updateTransitivity(parent->getId(), child->getId());
}

void PKB::insertProcModifyingVar(Procedure *proc, Variable *var) {
    modifiesProcTable.insertRelationship(proc->getId(), var->getId());
}

void PKB::insertStmtModifyingVar(Statement *stmt, Variable *var) {
    modifiesStmtTable.insertRelationship(stmt->getId(), var->getId());
}

void PKB::insertProcUsingVar(Procedure *proc, Variable *var) {
    usesProcTable.insertRelationship(proc->getId(), var->getId());
}

void PKB::insertStmtUsingVar(Statement *stmt, Variable *var) {
    usesStmtTable.insertRelationship(stmt->getId(), var->getId());
}

void PKB::insertCalls(Procedure *proc, const ProcName &called) {
    callsTable.insertRelationship(proc->getId(), called);
    callsStarTable.insertRelationship(proc->getId(), called);
    callsStarTable.updateTransitivity(proc->getId(), called);
}

void PKB::insertNext(Statement *previous, Statement *next) {
    nextTable.insertRelationship(previous->getId(), next->getId());
}

void PKB::insertNextBip(Statement *previous, Statement *next) {
    nextBipTable.insertRelationship(previous->getId(), next->getId());
}

void PKB::insertAssignPattern(Statement *stmt) {
    StmtIndex stmtIndex = stmt->getId();
    VarName varName = stmt->getVariable()->getId();

    PostfixAdapter postfix = PostfixAdapter(stmt->getExpressionLst());
    set<string> partialPatterns = postfix.createPartialPatterns();
    string exactPattern = postfix.createExactPattern();

    for (auto &pattern : partialPatterns) {
        partialAssignPatternTable.insertRelationship(
                stmtIndex,AssignPattern { WILDCARD, pattern });
        partialAssignPatternTable.insertRelationship(
                stmtIndex,AssignPattern { varName, pattern });
    }
    partialAssignPatternTable.insertRelationship(
            stmtIndex,AssignPattern { varName, WILDCARD });

    exactAssignPatternTable.insertRelationship(
            stmtIndex,AssignPattern { WILDCARD, exactPattern });
    exactAssignPatternTable.insertRelationship(
            stmtIndex,AssignPattern { varName, exactPattern });
    exactAssignPatternTable.insertRelationship(
            stmtIndex,AssignPattern { varName, WILDCARD });
}

void PKB::insertIfPattern(Statement *stmt) {
    for (auto &var : stmt->getExpressionVars()) {
        ifPatternTable.insertRelationship(stmt->getId(), var->getId());
    }
    ifPatternTable.insertRelationship(stmt->getId(), WILDCARD);
}

void PKB::insertWhilePattern(Statement *stmt) {
    for (auto &var : stmt->getExpressionVars()) {
        whilePatternTable.insertRelationship(stmt->getId(), var->getId());
    }
    whilePatternTable.insertRelationship(stmt->getId(), WILDCARD);
}

void PKB::insertBranchIn(Statement *fromStmt, Statement *toStmt) {
    branchInTable.insertRelationship(fromStmt->getId(), toStmt->getId());
}

void PKB::insertBranchBack(Statement *fromStmt, Statement *toStmt) {
    branchBackTable.insertRelationship(fromStmt->getId(), toStmt->getId());
}

// =============================================================================
// Query Processor
// =============================================================================

Iterator<ProcName> PKB::getAllProcs() { return procTable.getIds(); }

Iterator<VarName> PKB::getAllVars() { return varTable.getIds(); }

Iterator<ConstName> PKB::getAllConsts() { return constTable.getIds(); }

Iterator<StmtIndex> PKB::getAllStmts() { return statementTable.getIndices(); }

Iterator<StmtIndex> PKB::getAllStmts(const StatementType &type) {
    return statementTable.getIndices(type);
}

StatementType PKB::getStmtType(const StmtIndex &stmt) {
    return statementTable.getStmtType(stmt);
}

VarName PKB::getPrintVariable(const StmtIndex &printStmt) {
    auto stmt = statementTable.getStmt(printStmt);
    if (stmt->getStatementType() != StatementType::PRINT) {
        throw runtime_error(
            "This statement number does not refer to a print statement.");
    }
    return stmt->getVariable()->getId();
}

VarName PKB::getReadVariable(const StmtIndex &readStmt) {
    auto stmt = statementTable.getStmt(readStmt);
    if (stmt->getStatementType() != StatementType::READ) {
        throw runtime_error(
            "This statement number does not refer to a read statement.");
    }
    return stmt->getVariable()->getId();
}

ProcName PKB::getCallProcedure(const StmtIndex &callStmt) {
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

set<ProcName> PKB::getProcsUsingVar(const VarName &var) {
    return usesProcTable.getLHSRelationships(var);
}

set<StmtIndex> PKB::getStmtsUsingVar(const VarName &var) {
    return usesStmtTable.getLHSRelationships(var);
}

set<VarName> PKB::getVarsUsedByProc(const ProcName &proc) {
    return usesProcTable.getRHSRelationships(proc);
}

set<VarName> PKB::getVarsUsedByStmt(const StmtIndex &stmt) {
    return usesStmtTable.getRHSRelationships(stmt);
}

bool PKB::procUses(const ProcName &proc, const VarName &var) {
    return usesProcTable.isRelationship(proc, var);
}

bool PKB::stmtUses(const StmtIndex &stmt, const VarName &var) {
    return usesStmtTable.isRelationship(stmt, var);
}

// Calls =======================================================================

set<ProcName> PKB::getCalledProcs(const ProcName &caller) {
    return callsTable.getRHSRelationships(caller);
}

set<ProcName> PKB::getCalledStarProcs(const ProcName &caller) {
    return callsStarTable.getRHSRelationships(caller);
}

set<ProcName> PKB::getCallerProcs(const ProcName &called) {
    return callsTable.getLHSRelationships(called);
}

set<ProcName> PKB::getCallerStarProcs(const ProcName &called) {
    return callsStarTable.getLHSRelationships(called);
}

bool PKB::calls(const ProcName &caller, const ProcName &called) {
    return callsTable.isRelationship(caller, called);
}

bool PKB::callsStar(const ProcName &caller, const ProcName &called) {
    return callsStarTable.isRelationship(caller, called);
}

// Next ========================================================================

set<StmtIndex> PKB::getNextLines(const ProgLineIndex &line) {
    return nextTable.getRHSRelationships(line);
}

set<StmtIndex> PKB::getPreviousLines(const ProgLineIndex &line) {
    return nextTable.getLHSRelationships(line);
}

bool PKB::next(const ProgLineIndex &previousLine, const ProgLineIndex &nextLine) {
    return nextTable.isRelationship(previousLine, nextLine);
}

// NextBip =====================================================================

set<StmtIndex> PKB::getNextBipLines(const ProgLineIndex &line) {
    return nextBipTable.getRHSRelationships(line);
}

set<StmtIndex> PKB::getPreviousBipLines(const ProgLineIndex &line) {
    return nextBipTable.getLHSRelationships(line);
}

bool PKB::nextBip(const ProgLineIndex &previousLine, const ProgLineIndex &nextLine) {
    return nextBipTable.isRelationship(previousLine, nextLine);
}

ProgLineIndex PKB::getBranchInToLine(const ProgLineIndex &line) {
    return branchInTable.getFirstRHSRelationship(line, InvalidIndex);
}

set<ProgLineIndex> PKB::getBranchInFromLines(const ProgLineIndex &line) {
    return branchInTable.getLHSRelationships(line);
}

bool PKB::branchIn(const ProgLineIndex &from, const ProgLineIndex &to) {
    return branchInTable.isRelationship(from, to);
}

set<ProgLineIndex> PKB::getBranchBackToLines(const ProgLineIndex &line) {
    return branchBackTable.getRHSRelationships(line);
}

set<ProgLineIndex> PKB::getBranchBackFromLines(const ProgLineIndex &line) {
    return branchBackTable.getLHSRelationships(line);
}

bool PKB::branchBack(const ProgLineIndex &from, const ProgLineIndex &to) {
    return branchBackTable.isRelationship(from, to);
}

// Pattern =====================================================================

set<StmtIndex> PKB::getPartialAssignPatternStmts(const VarName &var,
                                                 const ExpressionList &exprList) {
    string pattern = PostfixAdapter(exprList).createExactPattern();
    AssignPattern record = AssignPattern { var, pattern };

    return partialAssignPatternTable.getLHSRelationships(record);
}

set<StmtIndex> PKB::getExactAssignPatternStmts(const VarName &var,
                                               const ExpressionList &exprList) {
    string pattern = PostfixAdapter(exprList).createExactPattern();
    AssignPattern record = AssignPattern { var, pattern };

    return exactAssignPatternTable.getLHSRelationships(record);
}

set<StmtIndex> PKB::getIfPatternStmts(const VarName &var) {
    return ifPatternTable.getLHSRelationships(var);
}

set<StmtIndex> PKB::getWhilePatternStmts(const VarName &var) {
    return whilePatternTable.getLHSRelationships(var);
}

bool PKB::partialAssignPattern(const StmtIndex &stmtIndex, const VarName &var,
                               const ExpressionList &exprList) {
    string pattern = PostfixAdapter(exprList).createExactPattern();
    AssignPattern record = AssignPattern { var, pattern };

    return partialAssignPatternTable.isRelationship(stmtIndex, record);
}

bool PKB::exactAssignPattern(const StmtIndex &stmtIndex, const VarName &var,
                             const ExpressionList &exprList) {
    string pattern = PostfixAdapter(exprList).createExactPattern();
    AssignPattern record = AssignPattern { var, pattern };

    return exactAssignPatternTable.isRelationship(stmtIndex, record);
}

bool PKB::ifPattern(const StmtIndex &stmtIndex, const VarName &var) {
    return ifPatternTable.isRelationship(stmtIndex, var);
}

bool PKB::whilePattern(const StmtIndex &stmtIndex, const VarName &var) {
    return whilePatternTable.isRelationship(stmtIndex, var);
}
