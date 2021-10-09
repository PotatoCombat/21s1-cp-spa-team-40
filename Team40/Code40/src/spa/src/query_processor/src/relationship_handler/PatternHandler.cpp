#include "query_processor/relationship_handler/PatternHandler.h"

Result PatternHandler::eval() {
    Result result;

    validate();

    Reference *stmtRef = patternClause->getFirstReference();
    Reference *varRef = patternClause->getSecondReference();
    string pattern = patternClause->getPattern();

    // temp change to pass iter1 test, will update later when moving to the new PKB
    if (pattern.at(0) == '_' && pattern.length() > 1) {
        pattern = pattern.substr(2, pattern.length() - 4);
    }
    // --------------------------------------------

    bool isSameAsModifies =
        stmtRef->getDeType() == DesignEntityType::IF ||
        varRef->getDeType() == DesignEntityType::WHILE ||
        patternClause->getPattern() == "_";

    // if same as modifies => use modifies handler
    if (isSameAsModifies) {
        Clause clause(ClauseType::MODIFIES_S, *stmtRef, *varRef);
        ModifiesStmtHandler handler(&clause, pkb);
        Result temp = handler.eval();
        // have to copy back because the Reference passed into clause is deleted
        // when this function finishes
        result.setValid(temp.isResultValid());
        if (temp.hasResultList1()) {
            result.setResultList1(stmtRef, temp.getResultList1());
        }
        if (temp.hasResultList2()) {
            result.setResultList2(varRef, temp.getResultList2());
        }
        return result;
    }

    // SYNONYM CONST
    if (varRef->getRefType() == ReferenceType::CONSTANT) {
        map<VALUE, VALUE_SET> stmtResults;
        set<int> stmts =
            pkb->getAssignsMatchingPattern(varRef->getValue(), pattern);
        for (auto stmt : stmts) {
            stmtResults[to_string(stmt)] = VALUE_SET{};
        }
        result.setResultList1(stmtRef, stmtResults);
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
        for (int assign : assigns) {
            if (pkb->assignMatchesPattern(assign, var, pattern)) {
                valid = true;
                if (stmtRef->getRefType() == ReferenceType::SYNONYM) {
                    related.insert(to_string(assign));
                }
            }
        }
        if (valid) {
            varResults[var] = related;
        }
    }

    result.setResultList1(stmtRef, stmtResults);

    if (varRef->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList2(varRef, varResults);
    }

    return result;
}

void PatternHandler::validate() {
    Reference *stmt = patternClause->getFirstReference();
    Reference *var = patternClause->getSecondReference();
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
