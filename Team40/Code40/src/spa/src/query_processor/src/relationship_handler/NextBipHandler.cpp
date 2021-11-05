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
    set<ProgLineIndex> prevBipLines = pkb->getPreviousBipLines(stoi(r2));
    vector<ProgLineIndex> toExplore(prevBipLines.size());
    copy(prevBipLines.begin(), prevBipLines.end(), toExplore.begin());
    while (!toExplore.empty()) {
        auto stmt = toExplore.back();
        toExplore.pop_back();
        if (isDummyStmt(stmt)) {
            for (auto j : pkb->getPreviousBipLines(stmt)) {
                toExplore.push_back(j);
            }
        } else {
            result.insert(to_string(stmt));
        }
    }
    return result;
}

set<string> NextBipHandler::getR2ClausedR1(string r1) {
    set<string> result;
    set<ProgLineIndex> nextBipLines = pkb->getNextBipLines(stoi(r1));
    vector<ProgLineIndex> toExplore(nextBipLines.size());
    copy(nextBipLines.begin(), nextBipLines.end(), toExplore.begin());
    while (!toExplore.empty()) {
        auto stmt = toExplore.back();
        toExplore.pop_back();
        if (isDummyStmt(stmt)) {
            for (auto j : pkb->getNextBipLines(stmt)) {
                toExplore.push_back(j);
            }
        } else {
            result.insert(to_string(stmt));
        }
    }
    return result;
}

bool NextBipHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->nextBip(stoi(r1), stoi(r2));
}