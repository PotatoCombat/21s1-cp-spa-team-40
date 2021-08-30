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

    vector<StatementIndex> getAllStmts();

    VarIndex insertVar(Var variable);
    ConstIndex insertConst(Const constant);
    ProcedureIndex insertProc(Procedure procedure);

    StatementIndex insertStmt(Statement statement);

private:
    typedef EntityTable<Var, VarIndex> VarTable;
    typedef EntityTable<Const, ConstIndex> ConstTable;
    typedef EntityTable<Procedure, ProcedureIndex> ProcedureTable;

    typedef StatementTable<Statement, StatementIndex> StatementTable;

    VarTable varTable;
    ConstTable constTable;
    ProcedureTable procTable;

    StatementTable statementTable;
};
