#include "query_processor/relationship_handler/ParentStarHandler.h"

ParentStarHandler::ParentStarHandler(Clause *clause, PKB *pkb,
                                     ResultCache *cache)
    : ClauseHandler(clause, pkb, cache, ClauseType::PARENT_T) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> ParentStarHandler::getR1ClauseR2(string r2) {
    set<string> res;
    for (int i : pkb->getParentStarStmts(stoi(r2))) {
        res.insert(to_string(i));
    }
    return res;
}

set<string> ParentStarHandler::getR2ClausedR1(string r1) {
    set<string> res;
    for (int i : pkb->getChildStarStmts(stoi(r1))) {
        res.insert(to_string(i));
    }
    return res;
}

bool ParentStarHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->parentStar(stoi(r1), stoi(r2));
}
