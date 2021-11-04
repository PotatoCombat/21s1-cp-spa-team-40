#include "query_processor/relationship_handler/NextBipStarHandler.h"

NextBipStarHandler::NextBipStarHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::NEXTBIP_T) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

void NextBipStarHandler::depthFirstSearch(
    ExplorationFunction explore, ProgLineIndex progLine, set<string> &result,
    unordered_set<ProgLineIndex> &visited,
    vector<ProgLineIndex> &validBranchLines, optional<ProgLineIndex> target) {

    if (!validBranchLines.empty() && validBranchLines.back() == progLine) {
        validBranchLines.pop_back();
    }

    // If while statement, mark as visited to prevent infinite loop
    if (!isDummyStmt(progLine) &&
        pkb->getStmtByIndex(progLine)->getStatementType() ==
            StatementType::WHILE) {
        visited.insert(progLine);
    }

    // Explore neighbours
    unordered_set<ProgLineIndex> toExplore =
        (this->*explore)(progLine, validBranchLines);
    for (ProgLineIndex neighbourLine : toExplore) {
        if (!isDummyStmt(neighbourLine)) {
            result.insert(to_string(neighbourLine));
        }
        if (target.has_value() && neighbourLine == target.value()) {
            result.insert(to_string(target.value()));
            return;
        }
        if (visited.find(neighbourLine) == visited.end()) {
            depthFirstSearch(explore, neighbourLine, result, visited,
                             validBranchLines, target);
        }
    }
}

set<string> NextBipStarHandler::getR2ClausedR1(string r1) {
    ExplorationFunction explore = &NextBipStarHandler::getNextBipLines;
    unordered_set<ProgLineIndex> visited;
    vector<ProgLineIndex> validBranchLines;
    set<string> result;
    depthFirstSearch(explore, stoi(r1), result, visited, validBranchLines,
                     nullopt);
    return result;
}

set<string> NextBipStarHandler::getR1ClauseR2(string r2) {
    ExplorationFunction explore = &NextBipStarHandler::getPreviousBipLines;
    unordered_set<ProgLineIndex> visited;
    vector<ProgLineIndex> validBranchLines;
    set<string> result;
    depthFirstSearch(explore, stoi(r2), result, visited, validBranchLines,
                     nullopt);
    return result;
}

bool NextBipStarHandler::isR1ClauseR2(string r1, string r2) {
    ExplorationFunction explore = &NextBipStarHandler::getNextBipLines;
    unordered_set<ProgLineIndex> visited;
    vector<ProgLineIndex> validBranchLines;
    set<string> result;
    depthFirstSearch(explore, stoi(r1), result, visited, validBranchLines,
                     stoi(r2));
    return result.count(r2);
}

unordered_set<ProgLineIndex> NextBipStarHandler::getNextBipLines(
    ProgLineIndex curLine, vector<ProgLineIndex> &validBranchBackLines) {
    set<ProgLineIndex> nextBipLines = pkb->getNextBipLines(curLine);
    unordered_set<ProgLineIndex> validNextBipLines;
    for (auto nextBipLine : nextBipLines) {
        // If BranchIn, add next lines to validBranchBackLines
        if (pkb->branchIn(curLine, nextBipLine)) {
            set<ProgLineIndex> branchBackLines = pkb->getNextLines(curLine);
            if (branchBackLines.size() > 1) {
                throw ClauseHandlerError(
                    "Encountered a call statement with more than one next "
                    "statement (syntactically impossible).");
            }
            if (!branchBackLines.empty()) {
                validBranchBackLines.push_back(*branchBackLines.begin());
            }
        } else if (pkb->branchBack(curLine, nextBipLine)) {
            // If invalid BranchBack, discard it
            if (!validBranchBackLines.empty() &&
                validBranchBackLines.back() != nextBipLine) {
                continue;
            }
        }
        validNextBipLines.insert(nextBipLine);
    }
    return validNextBipLines;
}

unordered_set<ProgLineIndex> NextBipStarHandler::getPreviousBipLines(
    ProgLineIndex curLine, vector<ProgLineIndex> &validBranchInLines) {
    set<ProgLineIndex> previousBipLines = pkb->getPreviousBipLines(curLine);
    unordered_set<ProgLineIndex> validPreviousBipLines;
    for (auto previousBipLine : previousBipLines) {
        // If BranchBack, update valid BranchIns
        if (pkb->branchBack(previousBipLine, curLine)) {
            unordered_set<ProgLineIndex> branchInLines =
                getBranchInLines(previousBipLine, curLine);
            if (!branchInLines.empty()) {
                validBranchInLines.push_back(*branchInLines.begin());
            }
        } else if (pkb->branchIn(previousBipLine, curLine)) {
            // If invalid BranchIn, discard it
            if (!validBranchInLines.empty() &&
                validBranchInLines.back() != previousBipLine) {
                continue;
            }
        }
        validPreviousBipLines.insert(previousBipLine);
    }
    return validPreviousBipLines;
}

unordered_set<ProgLineIndex>
NextBipStarHandler::getBranchInLines(ProgLineIndex branchBackFromLine,
                                     ProgLineIndex branchBackToLine) {
    unordered_set<ProgLineIndex> branchInLines;
    set<ProgLineIndex> prevLines = pkb->getPreviousLines(branchBackToLine);
    for (auto prevLine : prevLines) {
        Statement *prevStmt = pkb->getStmtByIndex(prevLine);
        if (prevStmt->getStatementType() != StatementType::CALL) {
            continue;
        }
        // If NextBip*(prevLine, branchBackFromLine), then prevLine is a valid
        // BranchIn
        ExplorationFunction explore = &NextBipStarHandler::getNextBipLines;
        unordered_set<ProgLineIndex> visited;
        vector<ProgLineIndex> validBranchLines;
        set<string> result;
        depthFirstSearch(explore, prevLine, result, visited, validBranchLines,
                         branchBackFromLine);
        if (result.count(to_string(branchBackFromLine))) {
            branchInLines.insert(prevLine);
        }
    }
    return branchInLines;
}
