#include "query_processor/relationship_handler/NextStarHandler.h"

NextStarHandler::NextStarHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::NEXT_T) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

/**
 * Gets all reference 1 values such that reference 2 is executed next of
 * reference 1 directly or indirectly where reference 1 and reference 2 are
 * program lines
 * @return all valid reference 1 values
 */
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
            if (visited.find(i) == visited.end() && !isDummyStmt(i)) {
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

/**
 * Gets all reference 2 values such that reference 2 is executed next of
 * reference 1 directly or indirectly where reference 1 and reference 2 are
 * program lines
 * @return all valid reference 2 values
 */
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
            if (visited.find(i) == visited.end() && !isDummyStmt(i)) {
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

/**
 * Checks that reference reference 2 is executed next of reference 1 directly or
 * indirectly where reference 1 and reference 2 are program lines
 * @return true if reference 2 is executed next of reference 1, false otherwise
 */
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
            if (visited.find(i) == visited.end() && !isDummyStmt(i)) {
                visited.insert(i); // Mark current as visited
                open.push(i);
            }
        }
    }

    return false;
}
