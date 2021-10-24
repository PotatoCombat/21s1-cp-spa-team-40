#include "query_processor/relationship_handler/AffectsStarHandler.h"

AffectsStarHandler::AffectsStarHandler(Clause* clause, PKB* pkb)
    : ClauseHandler(clause, pkb, ClauseType::AFFECTS_T) {
    validDesType1 = &ClauseHandler::ASSIGN_STMT_DES_SET;
    validDesType2 = &ClauseHandler::ASSIGN_STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> AffectsStarHandler::getR1ClauseR2(string r2) {
    ProgLineIndex line2 = stoi(r2);
    ProgLineIndex curr;

    if (pkb->getStmtType(line2) != StatementType::ASSIGN) {
        return { };
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

set<string> AffectsStarHandler::getR2ClausedR1(string r1) {
    ProgLineIndex line1 = stoi(r1);
    ProgLineIndex curr;

    if (pkb->getStmtType(line1) != StatementType::ASSIGN) {
        return { };
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

bool AffectsStarHandler::isR1ClauseR2(string r1, string r2) {
    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;

    ProgLineIndex line1 = stoi(r1);
    ProgLineIndex line2 = stoi(r2);
    ProgLineIndex curr;

    if (pkb->getStmtType(line1) != StatementType::ASSIGN
        || pkb->getStmtType(line2) != StatementType::ASSIGN) {
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
        return { };
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
                        case StatementType::IF:
                            case StatementType::WHILE:
                                break;
                        default:
                            if (pkb->stmtModifies(i, usedVar)) {
                                continue;
                            }
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
        return { };
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

    if (pkb->getStmtType(line1) != StatementType::ASSIGN
    || pkb->getStmtType(line2) != StatementType::ASSIGN) {
        return false;
    }

    // Should only have one var
    VarName modifiedVar = *pkb->getVarsModifiedByStmt(line1).begin();

    open.push(line1);

    while (!open.empty()) {
        curr = open.front();
        open.pop();

        // Explore neighbours
        for (ProgLineIndex i : pkb->getNextLines(curr)) {
            if (i == line2) {
                return pkb->stmtUses(i, modifiedVar);
            }

            if (visited.find(i) != visited.end()) {
                continue;
            }

            if (pkb->stmtModifies(i, modifiedVar)) {
                continue;
            }

            visited.insert(i); // Mark current as visited
            open.push(i);
        }
    }

    return false;
}
