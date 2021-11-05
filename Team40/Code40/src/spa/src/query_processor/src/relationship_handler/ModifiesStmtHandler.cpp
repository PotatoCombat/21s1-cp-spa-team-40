#include "query_processor/relationship_handler/ModifiesStmtHandler.h"

ModifiesStmtHandler::ModifiesStmtHandler(Clause *clause, PKB *pkb,
                                         ResultCache *cache)
    : ClauseHandler(clause, pkb, cache, ClauseType::MODIFIES_S) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::VARIABLE_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> ModifiesStmtHandler::getR1ClauseR2(string r2) {
    set<string> res;
    for (int i : pkb->getStmtsModifyingVar(r2)) {
        res.insert(to_string(i));
    }
    return res;
}

set<string> ModifiesStmtHandler::getR2ClausedR1(string r1) {
    return pkb->getVarsModifiedByStmt(stoi(r1));
}

bool ModifiesStmtHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->stmtModifies(stoi(r1), r2);
}
