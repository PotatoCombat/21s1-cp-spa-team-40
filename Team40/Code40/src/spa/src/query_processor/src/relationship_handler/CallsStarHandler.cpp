#include "query_processor/relationship_handler/CallsStarHandler.h"

CallsStarHandler::CallsStarHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::CALLS_T) {
    validDesType1 = &ClauseHandler::PROCEDURE_DES_SET;
    validDesType2 = &ClauseHandler::PROCEDURE_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> CallsStarHandler::getR1ClauseR2(string r2) {
    return pkb->getCallerStarProcs(r2);
}

set<string> CallsStarHandler::getR2ClausedR1(string r1) {
    return pkb->getCalledStarProcs(r1);
}

bool CallsStarHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->callsStar(r1, r2);
}
