#include "query_processor/relationship_handler/CallsStarHandler.h"

CallsStarHandler::CallsStarHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::CALLS_T) {
    validDesType1 = &ClauseHandler::PROCEDURE_DES_SET;
    validDesType2 = &ClauseHandler::PROCEDURE_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

/**
 * Gets all reference 1 values such that reference 1 calls reference 2 directly
 * or indirectly, where reference 1 and reference 2 are procedures
 * @return all valid reference 1 values
 */
set<string> CallsStarHandler::getR1ClauseR2(string r2) { return pkb->getCallerStarProcs(r2); }

/**
 * Gets all reference 2 values such that reference 1 is called by reference 2,
 * directly or indirectly, where reference 1 and reference 2 are procedures
 * @return all valid reference 2 values
 */
set<string> CallsStarHandler::getR2ClausedR1(string r1) { return pkb->getCalledStarProcs(r1); }

/**
 * checks whether reference 1 calls reference 2, directly or indirectly, where
 * reference 1 and reference 2 are procedures
 * @return true if reference 1 calls reference 2, false otherwise
 */
bool CallsStarHandler::isR1ClauseR2(string r1, string r2) { return pkb->callsStar(r1, r2); }
