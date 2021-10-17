#include "query_processor/relationship_handler/NextStarHandler.h"

NextStarHandler::NextStarHandler(Clause* clause, PKB* pkb)
    : ClauseHandler(clause, pkb, ClauseType::NEXT_T) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> NextStarHandler::getR1ClauseR2(string r2) {
    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;
    bool repeatedR2 = false;

    ProgLineIndex line2 = stoi(r2);
    ProgLineIndex curr;

    open.push(line2);

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
            else if (i == line2) {
                repeatedR2 = true;
            }
        }
    }

    set<string> res;
    for (ProgLineIndex i : visited) {
        if (i != line2 || repeatedR2) {
            res.insert(to_string(i));
        }
    }

    return res;
}

set<string> NextStarHandler::getR2ClausedR1(string r1) {
    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;

    bool repeatedR1 = false;

    ProgLineIndex line1 = stoi(r1);
    ProgLineIndex curr;

    open.push(line1);

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
            else if (i == line1) {
                repeatedR1 = true;
            }
        }
    }

    set<string> res;
    for (ProgLineIndex i : visited) {
        if (i != line1 || repeatedR1) {
            res.insert(to_string(i));
        }
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

        if (curr == line2) {
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
