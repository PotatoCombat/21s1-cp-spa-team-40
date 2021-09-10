#include "source_processor/FollowsExtractor.h"

FollowsExtractor::FollowsExtractor(PKB pkb)
    : pkb(std::move(pkb)),
      ctx(ExtractionContext::getInstance().getFollowsContext()){};

void FollowsExtractor::handleProgram(Program program) {
    for (Procedure procedure : program.getProcLst()) {
        statementLists.push_back(procedure.getStmtLst());
    }
    while (!statementLists.empty()) {
        vector<Statement *> statementList = statementLists.back();
        handleStatementList(statementList);
        statementLists.pop_back();
    }
}

void FollowsExtractor::handleStatementList(
    const vector<Statement *> &statementList) {
    for (Statement *statement : statementList) {
        handleStatement(statement);
    }
    ctx.clear();
}

void FollowsExtractor::handleStatement(Statement *statement) {
    for (Statement *prev : ctx.getAllEntities()) {
        pkb.insertFollows(prev, statement);
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