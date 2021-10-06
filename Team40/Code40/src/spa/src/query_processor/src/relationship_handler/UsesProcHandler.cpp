#include "query_processor/relationship_handler/UsesProcHandler.h"

UsesProcHandler::UsesProcHandler(Clause* clause, PKB* pkb)
    : ClauseHandler(clause, pkb, ClauseType::USES_P) {
    validDesType1 = &ClauseHandler::PROCEDURE_DES_SET;
    validDesType2 = &ClauseHandler::VARIABLE_DES_SET;
    validRefType1 = &ClauseHandler::NO_WILDCARD_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> UsesProcHandler::getR1ClauseR2(string r2) {
    return pkb->getProcsUsingVar(r2);
}

set<string> UsesProcHandler::getR2ClausedR1(string r1) {
    return pkb->getVarsUsedByProc(r1);
}

bool UsesProcHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->procUses(r1, r2);
}
