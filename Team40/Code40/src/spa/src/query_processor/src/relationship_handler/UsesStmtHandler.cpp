#include "query_processor/relationship_handler/UsesStmtHandler.h"

Result UsesStmtHandler::eval() {
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
        vector<string> stmtResults;
        set<int> stmts = pkb->getStmtsUsingVar(secondValue);
        for (auto stmt : stmts) {
            if (isDesTypeStmtType(firstReference->getDeType(), pkb->getStmtType(stmt))) {
                stmtResults.push_back(to_string(stmt));
            }
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
    vector<int> stmts;
    if (firstReference->getDeType() == DesignEntityType::STMT) {
        stmts = pkb->getAllStmts().asVector();
    } else {
        StatementType firstStmtType = desTypeToStmtType(firstReference->getDeType());
        stmts = pkb->getAllStmts(firstStmtType).asVector();
    }
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
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("ModifiesStmtHandler: first argument must be statememt type");
    }

    if (secondReference->getDeType() != DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("ModifiesStmtHandler: second argument must be variable type");
    }

    if (clause->getType() != ClauseType::USES_S) {
        throw ClauseHandlerError("ModifiesStmtHandler: relation type must be USES_S");
    }

    if (firstReference->getRefType() == ReferenceType::WILDCARD) {
        throw ClauseHandlerError("UsesStmtHandler: first argument cannot be wildcard");
    }
}
