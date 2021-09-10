#include "source_processor/DesignExtractor.h"

using namespace std;

DesignExtractor::DesignExtractor(PKB pkb) : pkb(pkb) {}

void DesignExtractor::handleProgram(Program program) {
    for (Procedure proc : program.getProcLst()) {
        handleProcedure(&proc);
    }
}

ProcIndex DesignExtractor::handleProcedure(Procedure *procedure) {
    ProcIndex procIndex = pkb.insertProc(procedure);
    ExtractionContext::getInstance().setCurrentProc(procIndex);
    for (Statement statement : procedure->getStmtLst()) {
        handleStatement(&statement);
    }
    return procIndex;
}

StmtIndex DesignExtractor::handleStatement(Statement *statement) {
    StmtIndex stmtIndex;
    switch (statement->getStatementType()) {
    case StatementType::ASSIGN:
        stmtIndex = handleAssignStatement(statement);
        break;
    case StatementType::CALL:
        stmtIndex = handleCallStatement(statement);
        break;
    case StatementType::IF:
        stmtIndex = handleIfStatement(statement);
        break;
    case StatementType::READ:
    case StatementType::PRINT:
    case StatementType::WHILE:
    default:
        throw runtime_error("Invalid StatementType!");
    }
    handleContextualRelationships(stmtIndex);
    return stmtIndex;
}

void DesignExtractor::handleContextualRelationships(StmtIndex stmtIndex) {
    optional<StmtIndex> prevStmtIndex =
        ExtractionContext::getInstance().getPrevStatement();
    optional<StmtIndex> parentStmtIndex =
        ExtractionContext::getInstance().getParentStatement();

// TODO: Fix extractors to work with Statement objects
//    if (prevStmtIndex.has_value()) {
//        pkb.insertFollows(prevStmtIndex.value(), stmtIndex);
//    }
//    if (parentStmtIndex.has_value()) {
//        pkb.insertParent(parentStmtIndex.value(), stmtIndex);
//    }

    ExtractionContext::getInstance().setPrevStatement(stmtIndex);
}

StmtIndex DesignExtractor::handleAssignStatement(Statement *assignStatement) {
    StmtIndex stmtIndex = pkb.insertStmt(assignStatement);
    Variable var = assignStatement->getVariable();
    VarIndex varIdx = pkb.insertVar(&var);
    //TODO: Use new API
    //pkb.insertStmtModifyingVar(stmtIndex, varIdx);
    return stmtIndex;
}

StmtIndex DesignExtractor::handleCallStatement(Statement *callStatement) {
    StmtIndex stmtIndex = pkb.insertStmt(callStatement);
    return stmtIndex;
}

StmtIndex DesignExtractor::handleIfStatement(Statement *ifStatement) {
    StmtIndex stmtIndex = pkb.insertStmt(ifStatement);
    Condition cond = ifStatement->getCondExpr();
    return stmtIndex;
}
