#include "UsesStmtHandler.h"

Result UsesStmtHandler::eval() {
    Result result;
    Reference *firstReference = relation->getFirstReference();
    Reference *secondReference = relation->getSecondReference();
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
        vector<string> stmtResults;
        set<int> stmts = pkb->getStmtsUsingVar(secondValue);
        for (auto stmt : stmts) {
            stmtResults.push_back(to_string(stmt));
        }
        result.setResultList1(firstReference, stmtResults);
        return result;
    }

    // CONSTANT SYNONYM
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::SYNONYM) {
        vector<string> varResults;
        set<string> vars = pkb->getVarsUsedByStmt(stoi(firstValue));
        for (auto var : vars) {
            varResults.push_back(var);
        }
        result.setResultList2(secondReference, varResults);
        return result;
    }

    // NEITHER IS CONSTANT, FIRST ARGUMENT NOT WILDCARD
    vector<string> stmtResults;
    vector<string> varResults;
    vector<int> stmts = pkb->getAllStmts().asVector();
    for (int stmt : stmts) {
        set<string> vars = pkb->getVarsUsedByStmt(stmt);
        if (vars.size() == 0) {
            continue;
        }
        stmtResults.push_back(to_string(stmt));
        for (auto var : vars) {
            if (find(varResults.begin(), varResults.end(), var) ==
                varResults.end()) {
                varResults.push_back(var);
            }
        }
    }

    result.setResultList1(firstReference, stmtResults);

    if (secondReference->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList2(secondReference, varResults);
    }

    return result;
}

void UsesStmtHandler::validate() {
    Reference *firstReference = relation->getFirstReference();
    Reference *secondReference = relation->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw RelationHandlerError("ModifiesStmtHandler: first argument must be statememt type");
    }

    if (secondReference->getDeType() != DesignEntityType::VARIABLE) {
        throw RelationHandlerError("ModifiesStmtHandler: second argument must be variable type");
    }

    if (relation->getType() != RelationType::USES_S) {
        throw RelationHandlerError("ModifiesStmtHandler: relation type must be USES_S");
    }

    if (firstReference->getRefType() == ReferenceType::WILDCARD) {
        throw RelationHandlerError("UsesStmtHandler: first argument cannot be wildcard");
    }
}
