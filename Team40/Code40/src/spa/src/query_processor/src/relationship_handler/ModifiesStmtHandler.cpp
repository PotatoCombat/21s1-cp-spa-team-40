#include "ModifiesStmtHandler.h"

Result ModifiesStmtHandler::eval() {
    Result result;
    Reference *firstReference = relation->getFirstReference();
    Reference *secondReference = relation->getSecondReference();
    string firstValue = firstReference->getValue();
    string secondValue = secondReference->getValue();

    if (firstReference->getRefType() == ReferenceType::WILDCARD) {
        throw RelationHandlerError("ModifiesStmtHandler: first argument cannot be wildcard");
    }

    /// CONSTANT CONSTANT
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        result.setValid(pkb->stmtModifies(stoi(firstValue), secondValue));
        return result;
    }

    // CONSTANT WILDCARD
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::WILDCARD) {
        result.setValid(pkb->getVarsModifiedByStmt(stoi(firstValue)).size() > 0);
        return result;
    }

    // SYNONYM CONSTANT
    if (firstReference->getRefType() == ReferenceType::SYNONYM &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        vector<string> stmtResults;
        set<int> stmts = pkb->getStmtsModifyingVar(secondValue);
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
        set<string> vars = pkb->getVarsModifiedByStmt(stoi(firstValue));
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
        set<string> vars = pkb->getVarsModifiedByStmt(stmt);
        if (vars.size() == 0) {
            continue;
        }
        stmtResults.push_back(to_string(stmt));
        
        for (auto var : vars) {
            if (find(varResults.begin(), varResults.end(), var) == varResults.end()) {
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
