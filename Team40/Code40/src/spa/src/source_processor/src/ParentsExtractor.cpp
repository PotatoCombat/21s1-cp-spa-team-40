#include "source_processor/ParentsExtractor.h"

ParentsExtractor::ParentsExtractor(PKB pkb)
    : pkb(pkb), ctx(ExtractionContext::getInstance().getParentContext()){};

void ParentsExtractor::handleProgram(Program program) {
    for (Procedure procedure : program.getProcLst()) {
        for (Statement *statement : procedure.getStmtLst()) {
            handleStatement(statement);
        }
    }
}

// Recursively handles Parent relationships by traversing statements in a
// depth-first manner
void ParentsExtractor::handleStatement(Statement *statement) {
    vector<Statement *> parents = ctx.getAllEntities();
    if (!parents.empty()) {
        for (Statement *parent : parents) {
            pkb.insertParent(parent, statement);
        }
    }
    StatementType type = statement->getStatementType();
    if (type == StatementType::IF) {
        ctx.push(statement);
        for (Statement *child : statement->getThenStmtLst()) {
            handleStatement(child);
        }
        for (Statement *child : statement->getElseStmtLst()) {
            handleStatement(child);
        }
        ctx.pop(statement);
    }
    if (type == StatementType::WHILE) {
        ctx.push(statement);
        for (Statement *child : statement->getThenStmtLst()) {
            handleStatement(child);
        }
        ctx.pop(statement);
    }
}