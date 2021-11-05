#include "query_processor/relationship_handler/CallsHandler.h"

CallsHandler::CallsHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::CALLS) {
    validDesType1 = &ClauseHandler::PROCEDURE_DES_SET;
    validDesType2 = &ClauseHandler::PROCEDURE_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> CallsHandler::getR1ClauseR2(string r2) {
    return pkb->getCallerProcs(r2);
}

set<string> CallsHandler::getR2ClausedR1(string r1) {
    return pkb->getCalledProcs(r1);
}

bool CallsHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->calls(r1, r2);
}
