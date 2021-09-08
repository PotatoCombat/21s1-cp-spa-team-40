#include "source_processor/DesignExtractor.h"

using namespace std;

#include "pkb/PKB.h"

DesignExtractor::DesignExtractor(PKB pkb) : pkb(pkb) {}

void DesignExtractor::handleProgram(Program program) {
    for (Procedure proc : program.getProcLst()) {
        handleProcedure(&proc);
    }
}

void DesignExtractor::handleProcedure(Procedure *procedure) {
    pkb.insertProc(procedure);
    for (Statement statement : procedure->getStmtLst()) {
        handleStatement(&statement);
    }
}

void DesignExtractor::handleStatement(Statement *statement) {
    switch (statement->getStatementType()) {
    case StatementType::ASSIGN:
        handleAssignStatement(statement);
    case StatementType::CALL:
    case StatementType::IF:
    case StatementType::READ:
    case StatementType::PRINT:
    case StatementType::WHILE:
    default:
        throw runtime_error("Invalid StatementType!");
    }
}

void DesignExtractor::handleAssignStatement(Statement *assignStatement) {
    Statement *stmt = assignStatement;
    StmtIndex stmtIdx = pkb.insertStmt(stmt);
    Variable var = assignStatement->getVariable();
    VarIndex varIdx = pkb.insertVar(&var);
    pkb.insertStmtModifyingVar(stmtIdx, varIdx);
}