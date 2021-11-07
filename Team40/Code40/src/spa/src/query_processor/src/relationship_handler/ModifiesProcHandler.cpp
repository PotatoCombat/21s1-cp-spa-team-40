#include "query_processor/relationship_handler/ModifiesProcHandler.h"

ModifiesProcHandler::ModifiesProcHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::MODIFIES_P) {
    validDesType1 = &ClauseHandler::PROCEDURE_DES_SET;
    validDesType2 = &ClauseHandler::VARIABLE_DES_SET;
    validRefType1 = &ClauseHandler::NO_WILDCARD_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

/**
 * Gets all reference 1 values such that reference 1 modifies reference 2 where
 * reference 1 is a procedure and reference 2 is a variable
 * @return all valid reference 1 values
 */
set<string> ModifiesProcHandler::getR1ClauseR2(string r2) { return pkb->getProcsModifyingVar(r2); }

/**
 * Gets all reference 2 values such that reference 2 is modified by reference 1
 * where reference 1 is a procedure and reference 2 is a variable
 * @return all valid reference 2 values
 */
set<string> ModifiesProcHandler::getR2ClausedR1(string r1) {
    return pkb->getVarsModifiedByProc(r1);
}

/**
 * Checks that reference 1 modifies reference 2 where reference 1 is a procedure
 * and reference 2 is a variable
 * @return true if reference 1 modifies reference 2, false otherwise
 */
bool ModifiesProcHandler::isR1ClauseR2(string r1, string r2) { return pkb->procModifies(r1, r2); }
