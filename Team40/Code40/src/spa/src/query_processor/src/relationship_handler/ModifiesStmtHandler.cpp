#include "query_processor/relationship_handler/ModifiesStmtHandler.h"

ModifiesStmtHandler::ModifiesStmtHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::MODIFIES_S) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::VARIABLE_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

/**
 * Gets all reference 1 values such that reference 1 modifies reference 2 where
 * reference 1 is a statement and reference 2 is a variable
 * @return all valid reference 1 values
 */
set<string> ModifiesStmtHandler::getR1ClauseR2(string r2) {
    set<string> res;
    for (int i : pkb->getStmtsModifyingVar(r2)) {
        res.insert(to_string(i));
    }
    return res;
}

/**
 * Gets all reference 2 values such that reference 2 is modified by reference 1
 * where reference 1 is a statement and reference 2 is a variable
 * @return all valid reference 2 values
 */
set<string> ModifiesStmtHandler::getR2ClausedR1(string r1) {
    return pkb->getVarsModifiedByStmt(stoi(r1));
}

/**
 * Checks that reference 1 modifies reference 2 where reference 1 is a statement
 * and reference 2 is a variable
 * @return true if reference 1 modifies reference 2, false otherwise
 */
bool ModifiesStmtHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->stmtModifies(stoi(r1), r2);
}
