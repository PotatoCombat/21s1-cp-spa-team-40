#include "query_processor/relationship_handler/FollowsStarHandler.h"

FollowsStarHandler::FollowsStarHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::FOLLOWS_T) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

/**
 * Gets all reference 1 values such that reference 1 follows reference 2,
 * directly or indirectly, where reference 1 and reference 2 are statements
 * @return all valid reference 1 values
 */
set<string> FollowsStarHandler::getR1ClauseR2(string r2) {
    set<string> res;
    for (int i : pkb->getPrecedingStarStmts(stoi(r2))) {
        res.insert(to_string(i));
    }
    return res;
}

/**
 * Gets all reference 2 values such that reference 1 is followed by reference 2,
 * directly or indirectly, where reference 1 and reference 2 are statements
 * @return all valid reference 2 values
 */
set<string> FollowsStarHandler::getR2ClausedR1(string r1) {
    set<string> res;
    for (int i : pkb->getFollowingStarStmts(stoi(r1))) {
        res.insert(to_string(i));
    }
    return res;
}

/**
 * checks whether reference 1 follows reference 2, directly or indirectly, where
 * reference 1 and reference 2 are statements
 * @return true if reference 1 follows reference 2, false otherwise
 */
bool FollowsStarHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->followsStar(stoi(r1), stoi(r2));
}
