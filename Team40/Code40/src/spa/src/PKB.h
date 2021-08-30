#pragma once

#include "Abstractions.h"
#include "EntityTable.h"
#include "Iterator.h"
#include "StatementTable.h"

using namespace std;

class PKB {
public:
    Iterator<VarIndex> getAllVars();
    Iterator<ConstIndex> getAllConsts();
    Iterator<ProcedureIndex> getAllProcs();

    Iterator<StatementIndex> getAllStmts();

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
