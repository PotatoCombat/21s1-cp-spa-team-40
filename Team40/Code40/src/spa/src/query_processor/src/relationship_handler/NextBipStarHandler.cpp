#include "query_processor/relationship_handler/NextBipStarHandler.h"

NextBipStarHandler::NextBipStarHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::NEXTBIP_T) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> NextBipStarHandler::getR1ClauseR2(string r2) {
    queue<ProgLineIndex> toExplore;
    unordered_set<ProgLineIndex> visited;

    toExplore.push(stoi(r2));

    ProgLineIndex curIndex;
    while (!toExplore.empty()) {
        curIndex = toExplore.front();
        toExplore.pop();

        // Explore neighbours
        for (ProgLineIndex index : pkb->getPreviousBipLines(curIndex)) {
            // Only add neighbours that haven't been visited
            if (visited.find(index) == visited.end()) {
                visited.insert(index); // Mark current as visited
                toExplore.push(index);
            }
        }
    }

    set<string> res;
    for (ProgLineIndex index : visited) {
        res.insert(to_string(index));
    }

    return res;
}

set<string> NextBipStarHandler::getR2ClausedR1(string r1) {
    queue<ProgLineIndex> toExplore;
    unordered_set<ProgLineIndex> visited;

    toExplore.push(stoi(r1));

    ProgLineIndex curIndex;
    while (!toExplore.empty()) {
        curIndex = toExplore.front();
        toExplore.pop();

        // Explore neighbours
        for (ProgLineIndex index : pkb->getNextLines(curIndex)) {
            // Only add neighbours that haven't been visited
            if (visited.find(index) == visited.end()) {
                visited.insert(index); // Mark current as visited
                toExplore.push(index);
            }
        }
    }

    set<string> res;
    for (ProgLineIndex index : visited) {
        res.insert(to_string(index));
    }

    return res;
}

bool NextBipStarHandler::isR1ClauseR2(string r1, string r2) {
    queue<ProgLineIndex> toExplore;
    unordered_set<ProgLineIndex> visited;

    ProgLineIndex leftIndex = stoi(r1);
    ProgLineIndex rightIndex = stoi(r2);

    toExplore.push(leftIndex);

    ProgLineIndex curIndex;
    while (!toExplore.empty()) {
        curIndex = toExplore.front();
        toExplore.pop();

        // Explore neighbours
        for (ProgLineIndex i : pkb->getNextLines(curIndex)) {
            if (i == rightIndex) {
                return true;
            }

            // Only add neighbours that haven't been visited
            if (visited.find(i) == visited.end()) {
                visited.insert(i); // Mark current as visited
                toExplore.push(i);
            }
        }
    }

    return false;
}
