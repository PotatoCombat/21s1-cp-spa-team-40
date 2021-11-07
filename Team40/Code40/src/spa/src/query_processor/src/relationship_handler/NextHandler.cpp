#include "query_processor/relationship_handler/NextHandler.h"

NextHandler::NextHandler(Clause *clause, PKB *pkb) : ClauseHandler(clause, pkb, ClauseType::NEXT) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

/**
 * Gets all reference 1 values such that reference 2 is executed next of
 * reference 1 where reference 1 and reference 2 are program lines
 * @return all valid reference 1 values
 */
set<string> NextHandler::getR1ClauseR2(string r2) {
    set<string> res;
    for (int i : pkb->getPreviousLines(stoi(r2))) {
        if (!isDummyStmt(i)) {
            res.insert(to_string(i));
        }
    }
    return res;
}

/**
 * Gets all reference 2 values such that reference 2 is executed next of
 * reference 1 where reference 1 and reference 2 are program lines
 * @return all valid reference 2 values
 */
set<string> NextHandler::getR2ClausedR1(string r1) {
    set<string> res;
    for (int i : pkb->getNextLines(stoi(r1))) {
        if (!isDummyStmt(i)) {
            res.insert(to_string(i));
        }
    }
    return res;
}

/**
 * Checks that reference reference 2 is executed next of reference 1 where
 * reference 1 and reference 2 are program lines
 * @return true if reference 2 is executed next of reference 1, false otherwise
 */
bool NextHandler::isR1ClauseR2(string r1, string r2) { return pkb->next(stoi(r1), stoi(r2)); }
