#include "query_processor/relationship_handler/AffectsStarHandler.h"

AffectsStarHandler::AffectsStarHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::AFFECTS_T) {
    validDesType1 = &ClauseHandler::ASSIGN_STMT_DES_SET;
    validDesType2 = &ClauseHandler::ASSIGN_STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

/**
 * Gets all reference 1 values such that reference 1 affects reference 2, directly or indirectly
 * where reference 1 and reference 2 are assign statements
 * @return all valid reference 1 values
 */
set<string> AffectsStarHandler::getR1ClauseR2(string r2) {
    ProgLineIndex line2 = stoi(r2);
    ProgLineIndex curr;

    if (pkb->getStmtType(line2) != StatementType::ASSIGN) {
        return {};
    }

    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;

    open.push(line2);

    while (!open.empty()) {
        curr = open.front();
        open.pop();

        // Explore neighbours
        for (ProgLineIndex i : getAffectingStmts(curr)) {
            if (visited.find(i) != visited.end()) {
                continue;
            }
            visited.insert(i); // Mark current as visited
            open.push(i);
        }
    }

    set<string> results;
    for (ProgLineIndex i : visited) {
        results.insert(to_string(i));
    }

    return results;
}

/**
 * Gets all reference 2 values such that reference 1 is affected by reference 2,
 * directly or indirectly where reference 1 and reference 2 are assign
 * statements
 * @return all valid reference 2 values
 */
set<string> AffectsStarHandler::getR2ClausedR1(string r1) {
    ProgLineIndex line1 = stoi(r1);
    ProgLineIndex curr;

    if (pkb->getStmtType(line1) != StatementType::ASSIGN) {
        return {};
    }

    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;

    open.push(line1);

    while (!open.empty()) {
        curr = open.front();
        open.pop();

        // Explore neighbours
        for (ProgLineIndex i : getAffectedStmts(curr)) {
            if (visited.find(i) != visited.end()) {
                continue;
            }
            visited.insert(i); // Mark current as visited
            open.push(i);
        }
    }

    set<string> results;
    for (ProgLineIndex i : visited) {
        results.insert(to_string(i));
    }

    return results;
}

/**
 * checks whether reference 1 affects reference 2, directly or indirectly, where
 * reference 1 and reference 2 are assign statements
 * @return true if reference 1 affects reference 2, false otherwise
 */
bool AffectsStarHandler::isR1ClauseR2(string r1, string r2) {
    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;

    ProgLineIndex line1 = stoi(r1);
    ProgLineIndex line2 = stoi(r2);
    ProgLineIndex curr;

    if (pkb->getStmtType(line1) != StatementType::ASSIGN ||
        pkb->getStmtType(line2) != StatementType::ASSIGN) {
        return false;
    }

    open.push(line1);

    while (!open.empty()) {
        curr = open.front();
        open.pop();

        // Explore neighbours
        for (ProgLineIndex i : getAffectedStmts(curr)) {
            if (i == line2) {
                return true;
            }

            if (visited.find(i) != visited.end()) {
                continue;
            }

            visited.insert(i); // Mark current as visited
            open.push(i);
        }
    }

    return false;
}

set<ProgLineIndex> AffectsStarHandler::getAffectingStmts(ProgLineIndex line2) {
    set<ProgLineIndex> results;
    ProgLineIndex curr;

    if (pkb->getStmtType(line2) != StatementType::ASSIGN) {
        return {};
    }

    set<VarName> usedVars = pkb->getVarsUsedByStmt(line2);

    for (auto &usedVar : usedVars) {
        queue<ProgLineIndex> open;
        unordered_set<ProgLineIndex> visited;

        open.push(line2);

        while (!open.empty()) {
            curr = open.front();
            open.pop();

            // Explore neighbours
            for (ProgLineIndex i : pkb->getPreviousLines(curr)) {
                if (visited.find(i) != visited.end()) {
                    continue;
                }
                visited.insert(i); // Mark current as visited

                switch (pkb->getStmtType(i)) {
                case StatementType::ASSIGN:
                    if (pkb->stmtModifies(i, usedVar)) {
                        results.insert(i);
                        continue;
                    }
                    break;
                case StatementType::IF:
                case StatementType::WHILE:
                    break;
                default:
                    if (pkb->stmtModifies(i, usedVar)) {
                        continue;
                    }
                    break;
                }
                open.push(i);
            }
        }
    }

    return results;
}

set<ProgLineIndex> AffectsStarHandler::getAffectedStmts(ProgLineIndex line1) {
    set<ProgLineIndex> results;

    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;
    ProgLineIndex curr;

    if (pkb->getStmtType(line1) != StatementType::ASSIGN) {
        return {};
    }

    // Should only have one var
    VarName modifiedVar = *pkb->getVarsModifiedByStmt(line1).begin();

    open.push(line1);

    while (!open.empty()) {
        curr = open.front();
        open.pop();

        // Explore neighbours
        for (ProgLineIndex i : pkb->getNextLines(curr)) {
            if (visited.find(i) != visited.end()) {
                continue;
            }
            visited.insert(i); // Mark current as visited

            switch (pkb->getStmtType(i)) {
            case StatementType::ASSIGN:
                if (pkb->stmtUses(i, modifiedVar)) {
                    results.insert(i);
                }
                if (pkb->stmtModifies(i, modifiedVar)) {
                    continue;
                }
                break;
            case StatementType::IF:
            case StatementType::WHILE:
                break;
            default:
                if (pkb->stmtModifies(i, modifiedVar)) {
                    continue;
                }
                break;
            }
            open.push(i);
        }
    }

    return results;
}

bool AffectsStarHandler::affects(ProgLineIndex line1, ProgLineIndex line2) {
    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;
    ProgLineIndex curr;

    if (pkb->getStmtType(line1) != StatementType::ASSIGN ||
        pkb->getStmtType(line2) != StatementType::ASSIGN) {
        return false;
    }

    // Should only have one var
    VarName modifiedVar = *pkb->getVarsModifiedByStmt(line1).begin();

    if (!pkb->stmtUses(line2, modifiedVar)) {
        return false;
    }

    open.push(line1);

    while (!open.empty()) {
        curr = open.front();
        open.pop();

        // Explore neighbours
        for (ProgLineIndex i : pkb->getNextLines(curr)) {
            if (i == line2) {
                return true;
            }

            if (visited.find(i) != visited.end()) {
                continue;
            }

            switch (pkb->getStmtType(i)) {
            case StatementType::IF:
            case StatementType::WHILE:
                break;
            default:
                if (pkb->stmtModifies(i, modifiedVar)) {
                    continue;
                }
                break;
            }

            visited.insert(i); // Mark current as visited
            open.push(i);
        }
    }

    return false;
}
