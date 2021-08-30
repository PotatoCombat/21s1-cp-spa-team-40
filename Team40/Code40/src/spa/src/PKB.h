#pragma once

#include <string>
#include <vector>

#include "Abstractions.h"
#include "EntityTable.h"
#include "StatementTable.h"

using namespace std;

class PKB {
public:
    vector<VarIndex> getAllVars();
    vector<ConstIndex> getAllConsts();
    vector<ProcedureIndex> getAllProcs();

    VarIndex insertVar(Var variable);
    ConstIndex insertConst(Const constant);
    ProcedureIndex insertProc(Procedure procedure);

private:
    typedef EntityTable<Var, VarIndex> VarTable;
    typedef EntityTable<Const, ConstIndex> ConstTable;
    typedef EntityTable<Procedure, ProcedureIndex> ProcedureTable;

    VarTable varTable;
    ConstTable constTable;
    ProcedureTable procTable;
};
