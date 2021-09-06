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
    Iterator<ProcIndex> getAllProcs();

    Iterator<StmtIndex> getAllStmts();

    VarIndex insertVar(Var variable);
    ConstIndex insertConst(Const constant);
    ProcIndex insertProc(Procedure procedure);

    StmtIndex insertStmt(Statement statement);

private:
    typedef EntityTable<Var, VarIndex> VarTable;
    typedef EntityTable<Const, ConstIndex> ConstTable;
    typedef EntityTable<Procedure, ProcIndex> ProcedureTable;

    typedef StatementTable<Statement, StmtIndex> StatementTable;

    VarTable varTable;
    ConstTable constTable;
    ProcedureTable procTable;

    StatementTable statementTable;
};
