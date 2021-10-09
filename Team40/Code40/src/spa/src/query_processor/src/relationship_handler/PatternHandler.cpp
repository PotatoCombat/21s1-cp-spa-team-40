#include "query_processor/relationship_handler/PatternHandler.h"

Result PatternHandler::eval() {
    Result result;

    validate();

    bool isSameAsModifies =
        patternClause->getStmt()->getDeType() == DesignEntityType::IF ||
        patternClause->getStmt()->getDeType() == DesignEntityType::WHILE ||
        patternClause->getPattern() == "_";

    // if same as modifies => use modifies handler
    if (isSameAsModifies) {
        Reference *stmt = patternClause->getStmt();
        Reference *variable = patternClause->getVar();
        Clause clause(ClauseType::MODIFIES_S, *stmt, *variable);
        ModifiesStmtHandler handler(&clause, pkb);
        Result temp = handler.eval();
        // have to copy back because the Reference passed into clause is deleted
        // when this function finishes
        result.setValid(temp.isResultValid());
        if (temp.hasResultList1()) {
            result.setResultList1(stmt, temp.getResultList1());
        }
        if (temp.hasResultList2()) {
            result.setResultList2(variable, temp.getResultList2());
        }
        return result;
    }

    Reference *assign = patternClause->getStmt();
    Reference *variable = patternClause->getVar();
    string pattern = patternClause->getPattern();

    // SYNONYM CONST
    if (variable->getRefType() == ReferenceType::CONSTANT) {
        map<VALUE, VALUE_SET> stmtResults;
        set<int> stmts =
            pkb->getAssignsMatchingPattern(variable->getValue(), pattern);
        for (auto stmt : stmts) {
            stmtResults[to_string(stmt)] = VALUE_SET{};
        }
        result.setResultList1(assign, stmtResults);
        return result;
    }

    // SYNONYM SYNONYM, SYNONYM WILDCARD
    map<VALUE, VALUE_SET> stmtResults;
    map<VALUE, VALUE_SET> varResults;
    vector<int> assigns = pkb->getAllStmts(StatementType::ASSIGN).asVector();
    vector<string> vars = pkb->getAllVars().asVector();

    for (int assign : assigns) {
        VALUE_SET related;
        bool valid = false;
        for (string var : vars) {
            if (pkb->assignMatchesPattern(assign, var, pattern)) {
                valid = true;
                related.insert(var);
            }
        }
        if (valid) {
            stmtResults[to_string(assign)] = related;
        }
    }
    
    for (string var : vars) {
        VALUE_SET related;
        bool valid = false;
        for (int assignStmt : assigns) {
            if (pkb->assignMatchesPattern(assignStmt, var, pattern)) {
                valid = true;
                if (assign->getRefType() == ReferenceType::SYNONYM) {
                    related.insert(to_string(assignStmt));
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

void PatternHandler::validate() {
    Reference *stmt = patternClause->getStmt();
    Reference *var = patternClause->getVar();
    set<DesignEntityType> validDesTypes = {DesignEntityType::ASSIGN,
                                           DesignEntityType::WHILE,
                                           DesignEntityType::IF};
    if (validDesTypes.count(stmt->getDeType()) == 0 ||
        stmt->getRefType() != ReferenceType::SYNONYM) {
        throw ClauseHandlerError(
            "AssignPatternHandler: statement must be a synonym assigment, if, or while");
    }

    if (var->getDeType() != DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "AssignPatternHandler: second argument must be variable");
    }
}
