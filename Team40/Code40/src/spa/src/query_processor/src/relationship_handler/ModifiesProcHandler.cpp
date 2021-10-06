#include "query_processor/relationship_handler/ModifiesProcHandler.h"

ModifiesProcHandler::ModifiesProcHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::MODIFIES_P) {
    validDesType1 = &ClauseHandler::PROCEDURE_DES_SET;
    validDesType2 = &ClauseHandler::VARIABLE_DES_SET;
    validRefType1 = &ClauseHandler::NO_WILDCARD_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> ModifiesProcHandler::getR1ClauseR2(string r2) {
    return pkb->getProcsModifyingVar(r2);
}

set<string> ModifiesProcHandler::getR2ClausedR1(string r1) {
    return pkb->getVarsModifiedByProc(r1);
}

bool ModifiesProcHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->procModifies(r1, r2);
}
