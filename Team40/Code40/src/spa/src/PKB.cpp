#include "PKB.h"

using namespace std;

Iterator<VarIndex> PKB::getAllVars() {
    return varTable.getIndices();
}

Iterator<ConstIndex> PKB::getAllConsts() {
    return constTable.getIndices();
}

Iterator<ProcedureIndex> PKB::getAllProcs() {
    return procTable.getIndices();
}

Iterator<StatementIndex> PKB::getAllStmts() {
    return statementTable.getIndices();
}

VarIndex PKB::insertVar(Var variable) {
    return varTable.insert(variable);
}

ConstIndex PKB::insertConst(Const constant) {
    return constTable.insert(constant);
}

ProcedureIndex PKB::insertProc(Procedure procedure) {
    return procTable.insert(procedure);
}

StatementIndex PKB::insertStmt(Statement statement) {
    return statementTable.insert(statement);
}
