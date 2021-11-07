#include "query_processor/relationship_handler/UsesProcHandler.h"

UsesProcHandler::UsesProcHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::USES_P) {
    validDesType1 = &ClauseHandler::PROCEDURE_DES_SET;
    validDesType2 = &ClauseHandler::VARIABLE_DES_SET;
    validRefType1 = &ClauseHandler::NO_WILDCARD_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

/**
 * Gets all reference 1 values such that reference 1 uses reference 2, where
 * reference 1 is a procedure and reference 2 is a variable
 * @return all valid reference 1 values
 */
set<string> UsesProcHandler::getR1ClauseR2(string r2) { return pkb->getProcsUsingVar(r2); }

/**
 * Gets all reference 2 values such that reference 2 is used by reference 1,
 * where reference 1 is a procedure and reference 2 is a variable
 * @return all valid reference 2 values
 */
set<string> UsesProcHandler::getR2ClausedR1(string r1) { return pkb->getVarsUsedByProc(r1); }

/**
 * checks whether reference 1 uses reference 2, where reference 1 is a statement and
 * reference 2 is a variable
 * @return true if reference 1 uses reference 2, false otherwise
 */
bool UsesProcHandler::isR1ClauseR2(string r1, string r2) { return pkb->procUses(r1, r2); }
