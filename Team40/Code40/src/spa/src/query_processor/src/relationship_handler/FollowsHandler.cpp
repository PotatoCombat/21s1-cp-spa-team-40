#include "query_processor/relationship_handler/FollowsHandler.h"

FollowsHandler::FollowsHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::FOLLOWS) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> FollowsHandler::getR1ClauseR2(string r2) {
    set<string> res;
    int stmt = pkb->getPrecedingStmt(stoi(r2));
    if (stmt != InvalidIndex) {
        res.insert(to_string(stmt));
    }
    return res;
}

set<string> FollowsHandler::getR2ClausedR1(string r1) {
    set<string> res;
    int stmt = pkb->getFollowingStmt(stoi(r1));
    if (stmt != InvalidIndex) {
        res.insert(to_string(stmt));
    }
    return res;
}

bool FollowsHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->follows(stoi(r1), stoi(r2));
}
