#include "query_processor/relationship_handler/NextStarHandler.h"

NextStarHandler::NextStarHandler(Clause* clause, PKB* pkb)
    : ClauseHandler(clause, pkb, ClauseType::FOLLOWS) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> NextStarHandler::getR1ClauseR2(string r2) {
    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;

    open.push(stoi(r2));

    ProgLineIndex curr;
    while (!open.empty()) {
        curr = open.front();
        open.pop();

        visited.insert(curr); // Mark current as visited

        // Explore neighbours
        for (ProgLineIndex i : pkb->getPreviousLines(curr)) {
            // Only add neighbours that haven't been visited
            if (visited.find(i) == visited.end()) {
                open.push(i);
            }
        }
    }

    set<string> res;
    for (ProgLineIndex i : visited) {
        res.insert(to_string(i));
    }
    return res;
}

set<string> NextStarHandler::getR2ClausedR1(string r1) {
    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;

    open.push(stoi(r1));

    ProgLineIndex curr;
    while (!open.empty()) {
        curr = open.front();
        open.pop();

        visited.insert(curr); // Mark current as visited

        // Explore neighbours
        for (ProgLineIndex i : pkb->getNextLines(curr)) {
            // Only add neighbours that haven't been visited
            if (visited.find(i) == visited.end()) {
                open.push(i);
            }
        }
    }

    set<string> res;
    for (ProgLineIndex i : visited) {
        res.insert(to_string(i));
    }
    return res;
}

bool NextStarHandler::isR1ClauseR2(string r1, string r2) {
    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;

    open.push(stoi(r1));

    ProgLineIndex curr;
    ProgLineIndex search = stoi(r2);

    while (!open.empty()) {
        curr = open.front();
        open.pop();

        if (curr == search) {
            return true;
        }

        visited.insert(curr); // Mark current as visited

        // Explore neighbours
        for (ProgLineIndex i : pkb->getNextLines(curr)) {
            // Only add neighbours that haven't been visited
            if (visited.find(i) == visited.end()) {
                open.push(i);
            }
        }
    }

    return false;
}
