#pragma once

#include <vector>

#include "Abstractions.h"
#include "PKB.h"

using namespace std;

vector<VarIndex> PKB::getAllVars() {
    return varTable.getIndices();
}

vector<ConstIndex> PKB::getAllConsts() {
    return constTable.getIndices();
}

vector<ProcedureIndex> PKB::getAllProcs() {
    return procTable.getIndices();
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
