#include "query_processor/relationship_handler/ParentStarHandler.h"

ParentStarHandler::ParentStarHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::PARENT_T) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

/**
 * Gets all reference 1 values such that reference 1 is a direct or indirect
 * parent of reference 2, where reference 1 and reference 2 are statements
 * @return all valid reference 1 values
 */
set<string> ParentStarHandler::getR1ClauseR2(string r2) {
    set<string> res;
    for (int i : pkb->getParentStarStmts(stoi(r2))) {
        res.insert(to_string(i));
    }
    return res;
}

/**
 * Gets all reference 2 values such that reference 2 is a direct or indirect
 * child of reference 1, where reference 1 and reference 2 are statements
 * @return all valid reference 2 values
 */
set<string> ParentStarHandler::getR2ClausedR1(string r1) {
    set<string> res;
    for (int i : pkb->getChildStarStmts(stoi(r1))) {
        res.insert(to_string(i));
    }
    return res;
}

/**
 * checks whether reference reference 1 is a direct or indirect parent of reference
 * 2, where reference 1 and reference 2 are statements
 * @return true if reference 1 is a parent of reference 2, false otherwise
 */
bool ParentStarHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->parentStar(stoi(r1), stoi(r2));
}
