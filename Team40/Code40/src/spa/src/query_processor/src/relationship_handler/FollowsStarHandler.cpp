#include "query_processor/relationship_handler/FollowsStarHandler.h"

FollowsStarHandler::FollowsStarHandler(Clause *clause, PKB *pkb,
                                       ResultCache *cache)
    : ClauseHandler(clause, pkb, cache, ClauseType::FOLLOWS_T) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> FollowsStarHandler::getR1ClauseR2(string r2) {
    set<string> res;
    for (int i : pkb->getPrecedingStarStmts(stoi(r2))) {
        res.insert(to_string(i));
    }
    return res;
}

set<string> FollowsStarHandler::getR2ClausedR1(string r1) {
    set<string> res;
    for (int i : pkb->getFollowingStarStmts(stoi(r1))) {
        res.insert(to_string(i));
    }
    return res;
}

bool FollowsStarHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->followsStar(stoi(r1), stoi(r2));
}
