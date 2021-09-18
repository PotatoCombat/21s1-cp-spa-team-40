#include "query_processor/relationship_handler/UsesStmtHandler.h"

Result UsesStmtHandler::eval(int ref1Index, int ref2Index) {
    Result result;
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    string firstValue = firstReference->getValue();
    string secondValue = secondReference->getValue();

    // assertions
    validate();

    /// CONSTANT CONSTANT
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        result.setValid(pkb->stmtUses(stoi(firstValue), secondValue));
        return result;
    }

    // CONSTANT WILDCARD
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::WILDCARD) {
        result.setValid(pkb->getVarsUsedByStmt(stoi(firstValue)).size() > 0);
        return result;
    }

    // SYNONYM CONSTANT
    if (firstReference->getRefType() == ReferenceType::SYNONYM &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        vector<ValueToPointersMap> stmtResults;
        set<int> stmts = pkb->getStmtsUsingVar(secondValue);
        for (auto stmt : stmts) {
            if (isDesTypeStmtType(firstReference->getDeType(),
                                  pkb->getStmtType(stmt))) {
                ValueToPointersMap map(to_string(stmt), POINTER_SET{});
                stmtResults.push_back(map);
            }
        }
        result.setResultList1(firstReference, stmtResults);
        return result;
    }

    // CONSTANT SYNONYM
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::SYNONYM) {
        vector<ValueToPointersMap> varResults;
        set<string> vars = pkb->getVarsUsedByStmt(stoi(firstValue));
        for (auto var : vars) {
            ValueToPointersMap map(var, POINTER_SET{});
            varResults.push_back(map);
        }
        result.setResultList2(secondReference, varResults);
        return result;
    }

    // NEITHER IS CONSTANT, FIRST ARGUMENT NOT WILDCARD
    // if first arg is SYNONYM
    if (firstReference->getRefType() != ReferenceType::WILDCARD) {
        vector<ValueToPointersMap> stmtResults;
        vector<int> stmts;
        if (firstReference->getDeType() == DesignEntityType::STMT) {
            stmts = pkb->getAllStmts().asVector();
        } else {
            StatementType firstStmtType =
                desTypeToStmtType(firstReference->getDeType());
            stmts = pkb->getAllStmts(firstStmtType).asVector();
        }

        for (int stmt : stmts) {
            set<string> vars = pkb->getVarsUsedByStmt(stmt);
            POINTER_SET related;
            bool valid = false;
            for (auto var : vars) {
                valid = true;
                if (secondReference->getRefType() == ReferenceType::SYNONYM) {
                    related.insert(make_pair(ref2Index, var));
                }
            }
            if (valid) {
                ValueToPointersMap map(to_string(stmt), related);
                stmtResults.push_back(map);
            }
        }

        result.setResultList1(firstReference, stmtResults);
    }

    // if second arg is SYNONYM
    if (secondReference->getRefType() != ReferenceType::WILDCARD) {
        vector<ValueToPointersMap> varResults;
        vector<string> vars = pkb->getAllVars().asVector();
        for (string var : vars) {
            set<int> stmts = pkb->getStmtsUsingVar(var);
            POINTER_SET related;
            bool valid = false;
            for (auto stmt : stmts) {
                if (isDesTypeStmtType(firstReference->getDeType(),
                                      pkb->getStmtType(stmt))) {
                    valid = true;
                    if (firstReference->getRefType() ==
                        ReferenceType::SYNONYM) {
                        related.insert(make_pair(ref1Index, to_string(stmt)));
                    }
                }
            }
            if (valid) {
                ValueToPointersMap map(var, related);
                varResults.push_back(map);
            }
        }

        result.setResultList2(secondReference, varResults);
    }

    return result;
}

void UsesStmtHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "ModifiesStmtHandler: first argument must be statememt type");
    }

    if (secondReference->getDeType() != DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "ModifiesStmtHandler: second argument must be variable type");
    }

    if (clause->getType() != ClauseType::USES_S) {
        throw ClauseHandlerError(
            "ModifiesStmtHandler: relation type must be USES_S");
    }

    if (firstReference->getRefType() == ReferenceType::WILDCARD) {
        throw ClauseHandlerError(
            "UsesStmtHandler: first argument cannot be wildcard");
    }
}
