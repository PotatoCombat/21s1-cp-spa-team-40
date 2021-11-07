#include "query_processor/relationship_handler/ParentHandler.h"

ParentHandler::ParentHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::PARENT) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::STMT_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

/**
 * Gets all reference 1 values such that reference 1 is a parent of reference 2
 * where reference 1 and reference 2 are statements
 * @return all valid reference 1 values
 */
set<string> ParentHandler::getR1ClauseR2(string r2) {
    set<string> res;
    int stmt = pkb->getParentStmt(stoi(r2));
    if (stmt != InvalidIndex) {
        res.insert(to_string(stmt));
    }
    return res;
}

/**
 * Gets all reference 2 values such that reference 2 is a child of reference 1
 * where reference 1 and reference 2 are statements
 * @return all valid reference 2 values
 */
set<string> ParentHandler::getR2ClausedR1(string r1) {
    set<string> res;
    for (int i : pkb->getChildStmts(stoi(r1))) {
        res.insert(to_string(i));
    }
    return res;
}

/**
 * Checks that reference reference 1 is a parent of reference 2 where reference
 * 1 and reference 2 are statements
 * @return true if reference 1 is a parent of reference 2, false otherwise
 */
bool ParentHandler::isR1ClauseR2(string r1, string r2) { return pkb->parent(stoi(r1), stoi(r2)); }
