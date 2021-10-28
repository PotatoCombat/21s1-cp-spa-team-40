#include "query_processor/relationship_handler/NextStarHandler.h"

NextStarHandler::NextStarHandler(Clause *clause, PKB *pkb, ResultCache *cache)
    : ClauseHandler(clause, pkb, cache, ClauseType::NEXT_T) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> NextStarHandler::getR1ClauseR2(string r2) {
    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;

    ProgLineIndex line2 = stoi(r2);
    ProgLineIndex curr;

    open.push(line2);

    while (!open.empty()) {
        curr = open.front();
        open.pop();

        // Explore neighbours
        for (ProgLineIndex i : pkb->getPreviousLines(curr)) {
            // Only add neighbours that haven't been visited
            if (visited.find(i) == visited.end()) {
                visited.insert(i); // Mark current as visited
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

    ProgLineIndex line1 = stoi(r1);
    ProgLineIndex curr;

    open.push(line1);

    while (!open.empty()) {
        curr = open.front();
        open.pop();


        // Explore neighbours
        for (ProgLineIndex i : pkb->getNextLines(curr)) {
            // Only add neighbours that haven't been visited
            if (visited.find(i) == visited.end()) {
                visited.insert(i); // Mark current as visited
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

    ProgLineIndex line1 = stoi(r1);
    ProgLineIndex line2 = stoi(r2);
    ProgLineIndex curr;

    open.push(line1);

    while (!open.empty()) {
        curr = open.front();
        open.pop();

        // Explore neighbours
        for (ProgLineIndex i : pkb->getNextLines(curr)) {
            if (i == line2) {
                return true;
            }

            // Only add neighbours that haven't been visited
            if (visited.find(i) == visited.end()) {
                visited.insert(i); // Mark current as visited
                open.push(i);
            }
        }
    }

    return false;
}
