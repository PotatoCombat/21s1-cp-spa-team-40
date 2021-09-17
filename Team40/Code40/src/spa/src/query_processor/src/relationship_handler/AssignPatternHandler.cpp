#include "query_processor/relationship_handler/AssignPatternHandler.h"

Result AssignPatternHandler::eval() {
	Result result;
    Reference *assign = patternClause->getStmt();
    Reference* variable = patternClause->getVar();
    string pattern = patternClause->getPattern();

    validate();

    // pattern = WILDCARD => ModifiesStmtHandler
    if (pattern == "_") {
        Clause clause(ClauseType::MODIFIES_S, *assign, *variable);
        ModifiesStmtHandler handler(&clause, pkb);
        Result temp = handler.eval();
        // have to copy back because the Reference passed into clause is deleted
        // when this function finishes
        if (temp.hasResultList1()) {
            result.setResultList1(assign, temp.getResultList1());
        }
        if (temp.hasResultList2()) {
            result.setResultList2(assign, temp.getResultList2());
        }
    }

    // SYNONYM CONST
    if (variable->getRefType() == ReferenceType::CONSTANT) {
        vector<string> stmtResults;
        set<int> stmts =
            pkb->getAssignsMatchingPattern(variable->getValue(), pattern);
        for (auto stmt : stmts) {
            stmtResults.push_back(to_string(stmt));
        }
        result.setResultList1(assign, stmtResults);
        return result;
    }

    // SYNONYM SYNONYM, SYNONYM WILDCARD
    vector<string> stmtResults;
    vector<string> varResults;
    set<int> stmtsSet;
    vector<string> vars = pkb->getAllVars().asVector();
    for (auto var : vars) {
        set<int> stmts = pkb->getAssignsMatchingPattern(var, pattern);
        if (stmts.size() > 0) {
            varResults.push_back(var);
            stmtsSet.insert(stmts.begin(), stmts.end());
        }
    }
    for (auto stmt : stmtsSet) {
        stmtResults.push_back(to_string(stmt));
    }

    result.setResultList1(assign, stmtResults);

    if (variable->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList2(variable, varResults);
    }

    return result;
}

void AssignPatternHandler::validate() {
    Reference *stmt = patternClause->getStmt();
    Reference *var = patternClause->getVar();
    if (stmt->getDeType() != DesignEntityType::ASSIGN || stmt->getRefType() != ReferenceType::SYNONYM) {
        throw ClauseHandlerError(
            "AssignPatternHandler: statement must be a synonym assignment");
    }

    if (var->getDeType() != DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "AssignPatternHandler: second argument must be variable");
    }
}
