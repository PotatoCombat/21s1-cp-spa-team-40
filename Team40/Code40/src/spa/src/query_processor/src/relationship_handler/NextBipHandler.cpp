#include "query_processor/relationship_handler/NextBipHandler.h"

NextBipHandler::NextBipHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::NEXTBIP) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> NextBipHandler::getR1ClauseR2(string r2) {
    set<string> result;
    for (ProgLineIndex index : pkb->getPreviousBipLines(stoi(r2))) {
        result.insert(to_string(index));
    }
    return result;
}

set<string> NextBipHandler::getR2ClausedR1(string r1) {
    set<string> result;
    for (ProgLineIndex index : pkb->getNextBipLines(stoi(r1))) {
        result.insert(to_string(index));
    }
    return result;
}

bool NextBipHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->nextBip(stoi(r1), stoi(r2));
}