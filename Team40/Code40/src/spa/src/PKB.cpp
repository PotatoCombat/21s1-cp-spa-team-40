#pragma once

#include <vector>

#include "Abstractions.h"
#include "PKB.h"

using namespace std;

vector<Var> PKB::getAllVars() {
    return varTable.getEntities();
}

VarIndex PKB::insertVar(Var variable) {
    return varTable.insert(variable);
}

vector<Const> PKB::getAllConsts() {
    return constTable.getEntities();
}

ConstIndex PKB::insertConst(Const constant) {
    return constTable.insert(constant);
}
vector<Procedure> PKB::getAllProcs() {
    return procTable.getEntities();
}

ProcedureIndex PKB::insertProc(Procedure procedure) {
    return procTable.insert(procedure);
}
