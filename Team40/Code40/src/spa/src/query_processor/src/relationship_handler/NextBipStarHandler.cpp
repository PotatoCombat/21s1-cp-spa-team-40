#include "query_processor/relationship_handler/NextBipStarHandler.h"

NextBipStarHandler::NextBipStarHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::NEXTBIP_T) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> NextBipStarHandler::breadthFirstSearch(ExplorationFunction explore,
                                                   const string &r) {
    queue<ProgLineIndex> toExplore;
    unordered_set<ProgLineIndex> visitedWhileStmts;
    set<string> result;

    toExplore.push(stoi(r));

    ProgLineIndex curIndex;
    unordered_set<ProgLineIndex> validBranchLines;
    while (!toExplore.empty()) {
        curIndex = toExplore.front();
        toExplore.pop();
        // If while statement, mark as visited to prevent infinite loop
        if (pkb->getStmtByIndex(curIndex)->getStatementType() ==
            StatementType::WHILE) {
            visitedWhileStmts.insert(curIndex);
        }

        // Explore neighbours
        for (ProgLineIndex index :
             (this->*explore)(curIndex, validBranchLines)) {
            if (visitedWhileStmts.find(index) == visitedWhileStmts.end()) {
                toExplore.push(index);
            }
            result.insert(to_string(index));
        }
    }

    return result;
}

set<string> NextBipStarHandler::getR2ClausedR1(string r1) {
    ExplorationFunction explore = &NextBipStarHandler::getNextBipLines;
    return breadthFirstSearch(explore, r1);
}

set<string> NextBipStarHandler::getR1ClauseR2(string r2) {
    ExplorationFunction explorationFunction =
        &NextBipStarHandler::getPreviousBipLines;
    return breadthFirstSearch(explorationFunction, r2);
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
        for (ProgLineIndex i : pkb->getNextBipLines(curIndex)) {
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

set<ProgLineIndex> NextBipStarHandler::getNextBipLines(
    ProgLineIndex curLine, unordered_set<ProgLineIndex> &validBranchBackLines) {
    set<ProgLineIndex> nextBipLines = pkb->getNextBipLines(curLine);
    set<ProgLineIndex> validNextBipLines;
    for (auto nextBipLine : nextBipLines) {
        // If BranchIn, add next lines to validBranchBackLines
        if (pkb->branchIn(curLine, nextBipLine)) {
            set<ProgLineIndex> branchBackLines = pkb->getNextLines(curLine);
            validBranchBackLines.insert(branchBackLines.begin(),
                                        branchBackLines.end());
        }
        // If invalid BranchBack, filter it out
        if (pkb->branchBack(curLine, nextBipLine)) {
            if (validBranchBackLines.find(nextBipLine) !=
                validBranchBackLines.end()) {
                validBranchBackLines.erase(nextBipLine);
            } else {
                continue;
            }
        }
        validNextBipLines.insert(nextBipLine);
    }
    return validNextBipLines;
}

set<ProgLineIndex> NextBipStarHandler::getPreviousBipLines(
    ProgLineIndex curLine, unordered_set<ProgLineIndex> &validBranchInLines) {
    // TODO: Add additional filter logic here
    set<ProgLineIndex> previousBipLines = pkb->getPreviousBipLines(curLine);
    set<ProgLineIndex> validPreviousBipLines;
    for (auto previousBipLine : previousBipLines) {
        // If BranchBack, add prev lines to validBranchInLines
        if (pkb->branchBack(previousBipLine, curLine)) {
            set<ProgLineIndex> branchInLInes = pkb->getPreviousLines(curLine);
            validBranchInLines.insert(branchInLInes.begin(),
                                      branchInLInes.end());
        }
        // If invalid BranchIn, filter it out
        if (pkb->branchIn(previousBipLine, curLine)) {
            if (validBranchInLines.find(curLine) != validBranchInLines.end()) {
                validBranchInLines.erase(curLine);
            } else {
                continue;
            }
        }
        validPreviousBipLines.insert(previousBipLine);
    }
    return validPreviousBipLines;
}
