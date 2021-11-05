#include "query_processor/relationship_handler/UsesStmtHandler.h"

UsesStmtHandler::UsesStmtHandler(Clause *clause, PKB *pkb)
    : ClauseHandler(clause, pkb, ClauseType::USES_S) {
    validDesType1 = &ClauseHandler::STMT_DES_SET;
    validDesType2 = &ClauseHandler::VARIABLE_DES_SET;
    validRefType1 = &ClauseHandler::ALL_VALID_REF;
    validRefType2 = &ClauseHandler::ALL_VALID_REF;
}

set<string> UsesStmtHandler::getR1ClauseR2(string r2) {
    set<string> res;
    for (int i : pkb->getStmtsUsingVar(r2)) {
        res.insert(to_string(i));
    }
    return res;
}

set<string> UsesStmtHandler::getR2ClausedR1(string r1) {
    return pkb->getVarsUsedByStmt(stoi(r1));
}

bool UsesStmtHandler::isR1ClauseR2(string r1, string r2) {
    return pkb->stmtUses(stoi(r1), r2);
}
