#include "source_processor/design_extractor/FollowsExtractor.h"

FollowsExtractor::FollowsExtractor(PKB *pkb)
    : pkb(pkb), ctx(ExtractionContext::getInstance().getFollowsContext()){};

void FollowsExtractor::extract(Program *program) {
    for (Procedure *procedure : program->getProcLst()) {
        statementLists.push_back(procedure->getStmtLst());
    }
    while (!statementLists.empty()) {
        vector<Statement *> statementList = statementLists.back();
        statementLists.pop_back();
        extractStatementList(statementList);
    }
}

void FollowsExtractor::extractStatementList(vector<Statement *> statementList) {
    for (Statement *statement : statementList) {
        extractStatement(statement);
    }
    ctx.clear();
}

void FollowsExtractor::extractStatement(Statement *statement) {
    for (Statement *prev : ctx.getAllEntities()) {
        pkb->insertFollows(prev, statement);
    }
    StatementType type = statement->getStatementType();
    if (type == StatementType::IF || type == StatementType::WHILE) {
        statementLists.push_back(statement->getThenStmtLst());
    }
    if (type == StatementType::IF) {
        statementLists.push_back(statement->getElseStmtLst());
    }
    ctx.push(statement);
}