#include "query_processor/relationship_handler/NextHandler.h"

NextHandler::NextHandler(Clause *clause, PKB *pkb, ResultCache *cache)
    : ClauseHandler(clause, pkb, cache, ClauseType::NEXT) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> NextHandler::getR1ClauseR2(string r2) {
    set<string> res;
    for (int i : pkb->getPreviousLines(stoi(r2))) {
        if (!isDummyStmt(i)) {
            res.insert(to_string(i));
        }
    }
    return res;
}

set<string> NextHandler::getR2ClausedR1(string r1) {
    set<string> res;
    for (int i : pkb->getNextLines(stoi(r1))) {
        if (!isDummyStmt(i)) {
            res.insert(to_string(i));
        }
    }
    return res;
}

bool NextHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->next(stoi(r1), stoi(r2));
}
