#include "query_processor/relationship_handler/AssignPatternHandler.h"

Result AssignPatternHandler::eval(int ref1Index, int ref2Index) {
    Result result;
    Reference *assign = patternClause->getStmt();
    Reference *variable = patternClause->getVar();
    string pattern = patternClause->getPattern();

    validate();

    // pattern = WILDCARD => ModifiesStmtHandler
    if (pattern == "_") {
        Clause clause(ClauseType::MODIFIES_S, *assign, *variable);
        ModifiesStmtHandler handler(&clause, pkb);
        Result temp = handler.eval(ref1Index, ref2Index);
        // have to copy back because the Reference passed into clause is deleted
        // when this function finishes
        result.setValid(temp.isResultValid());
        if (temp.hasResultList1()) {
            result.setResultList1(assign, temp.getResultList1());
        }
        if (temp.hasResultList2()) {
            result.setResultList2(assign, temp.getResultList2());
        }
    }

    // SYNONYM CONST
    if (variable->getRefType() == ReferenceType::CONSTANT) {
        VALUE_TO_POINTERS_MAP stmtResults;
        set<int> stmts =
            pkb->getAssignsMatchingPattern(variable->getValue(), pattern);
        for (auto stmt : stmts) {
            stmtResults[to_string(stmt)] = POINTER_SET{};
        }
        result.setResultList1(assign, stmtResults);
        return result;
    }

    // SYNONYM SYNONYM, SYNONYM WILDCARD
    VALUE_TO_POINTERS_MAP stmtResults;
    VALUE_TO_POINTERS_MAP varResults;
    vector<int> assigns = pkb->getAllStmts(StatementType::ASSIGN).asVector();
    vector<string> vars = pkb->getAllVars().asVector();

    for (int assign : assigns) {
        POINTER_SET related;
        bool valid = false;
        for (string var : vars) {
            if (pkb->assignMatchesPattern(assign, var, pattern)) {
                valid = true;
                if (variable->getRefType() == ReferenceType::SYNONYM) {
                    related.insert(make_pair(ref2Index, var));
                }
            }
        }
        if (valid) {
            stmtResults[to_string(assign)] = related;
        }
    }
    
    for (string var : vars) {
        POINTER_SET related;
        bool valid = false;
        for (int assignStmt : assigns) {
            if (pkb->assignMatchesPattern(assignStmt, var, pattern)) {
                valid = true;
                if (assign->getRefType() == ReferenceType::SYNONYM) {
                    related.insert(make_pair(ref1Index, to_string(assignStmt)));
                }
            }
        }
        if (valid) {
            varResults[var] = related;
        }
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
    if (stmt->getDeType() != DesignEntityType::ASSIGN ||
        stmt->getRefType() != ReferenceType::SYNONYM) {
        throw ClauseHandlerError(
            "AssignPatternHandler: statement must be a synonym assignment");
    }

    if (var->getDeType() != DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "AssignPatternHandler: second argument must be variable");
    }
}
