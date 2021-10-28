#include "query_processor/relationship_handler/ParentHandler.h"

ParentHandler::ParentHandler(Clause *clause, PKB *pkb, ResultCache *cache)
    : ClauseHandler(clause, pkb, cache, ClauseType::PARENT) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> ParentHandler::getR1ClauseR2(string r2) {
    set<string> res;
    int stmt = pkb->getParentStmt(stoi(r2));
    if (stmt != -1) {
        res.insert(to_string(stmt));
    }
    return res;
}

set<string> ParentHandler::getR2ClausedR1(string r1) {
    set<string> res;
    for (int i : pkb->getChildStmts(stoi(r1))) {
        res.insert(to_string(i));
    }
    return res;
}

bool ParentHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->parent(stoi(r1), stoi(r2));
}
