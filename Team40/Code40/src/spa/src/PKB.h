#pragma once

#include <string>
#include <vector>

#include "Abstractions.h"
#include "EntityTable.h"

using namespace std;

class PKB {
public:
    VarIndex insertVar(Var variable);
    ConstIndex insertConst(Const constant);
    ProcedureIndex insertProc(Procedure procedure);

    vector<Var> getAllVars();
    vector<Const> getAllConsts();
    vector<Procedure> getAllProcs();

private:
    typedef EntityTable<Var, VarIndex> VarTable;
    typedef EntityTable<Const, ConstIndex> ConstTable;
    typedef EntityTable<Procedure, ProcedureIndex> ProcedureTable;

    VarTable varTable;
    ConstTable constTable;
    ProcedureTable procTable;
};
