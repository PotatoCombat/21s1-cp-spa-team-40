#include "query_processor/relationship_handler/AffectsHandler.h"

AffectsHandler::AffectsHandler(Clause* clause, PKB* pkb)
    : ClauseHandler(clause, pkb, ClauseType::AFFECTS) {
    validDesType1 = &ClauseHandler::ASSIGN_STMT_DES_SET;
    validDesType2 = &ClauseHandler::ASSIGN_STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> AffectsHandler::getR1ClauseR2(string r2) {
    ProgLineIndex line2 = stoi(r2);

    if (pkb->getStmtType(line2) != StatementType::ASSIGN) {
        return {};
    }

    set<VarName> usedVars = pkb->getVarsUsedByStmt(line2);

    return breathFirstSearch(line2, usedVars, true);
}

set<string> AffectsHandler::getR2ClausedR1(string r1) {
    ProgLineIndex line1 = stoi(r1);

    if (pkb->getStmtType(line1) != StatementType::ASSIGN) {
        return {};
    }

    set<VarName> modifiedVars = pkb->getVarsModifiedByStmt(line1);

    // Should only have one var
    if (modifiedVars.size() != 1) {
        throw runtime_error("Invalid modifiedVars size");
    }

    return breathFirstSearch(line1, modifiedVars, false);
}

set<string> AffectsHandler::breathFirstSearch(const ProgLineIndex line, const set<VarName> &vars, bool isFindingR1) {
    set<string> results;
    ProgLineIndex curr;

    for (auto &var : vars) {
        queue<ProgLineIndex> open;
        unordered_set<ProgLineIndex> visited;

        open.push(line);

        while (!open.empty()) {
            curr = open.front();
            open.pop();

            // Explore neighbours
            exploreNeighbours(isFindingR1, curr, var, visited, open, results);
        }
    }

    return results;

}

void AffectsHandler::exploreNeighbours(bool isFindingR1,
                                       const ProgLineIndex currLine,
                                       const VarName currVar,
                                       unordered_set<ProgLineIndex> &visited,
                                       queue<ProgLineIndex> &open,
                                       set<string> &results) {
    bool (PKB::*checkRelationship)(ProgLineIndex, VarName) =
        isFindingR1 ? &PKB::stmtModifies : &PKB::stmtUses;

    set<ProgLineIndex> (PKB::*getNeighbours)(ProgLineIndex) =
        isFindingR1 ? &PKB::getPreviousLines : &PKB::getNextLines;

    for (ProgLineIndex i : (pkb->*getNeighbours)(currLine)) {
        if (visited.find(i) != visited.end()) {
            continue;
        }
        visited.insert(i); // Mark current as visited

        switch (pkb->getStmtType(i)) {
        case StatementType::ASSIGN:
            if ((pkb->*checkRelationship)(i, currVar)) {
                results.insert(to_string(i));
            }
            if (pkb->stmtModifies(i, currVar)) {
                continue;
            }
            break;
        case StatementType::IF:
        case StatementType::WHILE:
            break;
        default:
            if (pkb->stmtModifies(i, currVar)) {
                continue;
            }
            break;
        }
        open.push(i);
    }
}


bool AffectsHandler::isR1ClauseR2(string r1, string r2) {
    queue<ProgLineIndex> open;
    unordered_set<ProgLineIndex> visited;

    ProgLineIndex line1 = stoi(r1);
    ProgLineIndex line2 = stoi(r2);
    ProgLineIndex curr;

    if (pkb->getStmtType(line1) != StatementType::ASSIGN
        || pkb->getStmtType(line2) != StatementType::ASSIGN) {
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
