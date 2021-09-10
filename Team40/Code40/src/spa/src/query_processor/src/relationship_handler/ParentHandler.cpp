#include "ParentHandler.h"

Result ParentHandler::eval() {
    Result result;
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    string firstStmt = firstReference->getValue();
    string secondStmt = secondReference->getValue();

    // Todo: handle stmts by different design enity types
    // Todo: use variable instead of magic number -1

    // assertions
    validate();

    // WILDCARD WILDCARD
    if (firstReference->getRefType() == ReferenceType::WILDCARD &&
        secondReference->getRefType() == ReferenceType::WILDCARD) {
        vector<int> allStmts = pkb->getAllStmts().asVector();
        for (auto stmt : allStmts) {
            if (pkb->getParentStmt(stmt) != -1) {
                result.setValid(true);
                return result;
            }
        }
        result.setValid(false);
        return result;
    }

    /// CONSTANT CONSTANT
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        result.setValid(pkb->parent(stoi(firstStmt), stoi(secondStmt)));
        return result;
    }

    // CONSTANT WILDCARD
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::WILDCARD) {
        result.setValid(pkb->getChildStmts(stoi(firstStmt)).size() > 0);
        return result;
    }

    // WILDCARD CONSTANT
    if (firstReference->getRefType() == ReferenceType::WILDCARD &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        result.setValid(pkb->getParentStmt(stoi(secondStmt)) != -1);
        return result;
    }

    // SYNONYM CONSTANT
    if (firstReference->getRefType() == ReferenceType::SYNONYM &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        vector<string> firstStmtResults;
        int parentStmt = pkb->getParentStmt(stoi(secondStmt));
        if (parentStmt != -1 &&
            isDesTypeStmtType(firstReference->getDeType(), pkb->getStmtType(parentStmt))) {
            firstStmtResults.push_back(to_string(parentStmt));
        }
        result.setResultList1(firstReference, firstStmtResults);
        return result;
    }

    // CONSTANT SYNONYM
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::SYNONYM) {
        vector<string> secondStmtResults;
        set<int> childStmts = pkb->getChildStmts(stoi(firstStmt));
        for (auto childStmt : childStmts) {
            if (isDesTypeStmtType(secondReference->getDeType(), pkb->getStmtType(childStmt))) {
                secondStmtResults.push_back(to_string(childStmt));
            }
        }
        result.setResultList2(secondReference, secondStmtResults);
        return result;
    }

    // NEITHER IS CONSTANT
    vector<string> firstStmtResults;
    vector<string> secondStmtResults;
    vector<int> parentStmts;
    if (firstReference->getDeType() == DesignEntityType::STMT) {
        parentStmts = pkb->getAllStmts().asVector();
    } else {
        StatementType firstStmtType = desTypeToStmtType(firstReference->getDeType());
        parentStmts = pkb->getAllStmts(firstStmtType).asVector();
    }

    for (int parentStmt : parentStmts) {
        set<int> childStmts = pkb->getChildStmts(parentStmt);
        if (childStmts.size() == 0) {
            continue;
        }
        bool hasMatchingChild = false;
        
        for (auto childStmt : childStmts) {
            if (isDesTypeStmtType(secondReference->getDeType(), pkb->getStmtType(childStmt))) {
                secondStmtResults.push_back(to_string(childStmt));
                hasMatchingChild = true;
            }
        }  

        if (hasMatchingChild) {
            firstStmtResults.push_back(to_string(parentStmt));
        }
    }

    if (firstReference->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList1(firstReference, firstStmtResults);
    }

    if (secondReference->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList2(secondReference, secondStmtResults);
    }

    return result;
}

void ParentHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("ParentHandler: first argument must be statement type");
    }

    if (secondReference->getDeType() == DesignEntityType::PROCEDURE ||
        secondReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("ParentHandler: second argument must be statement type");
    }

    if (clause->getType() != ClauseType::PARENT) {
        throw ClauseHandlerError("ParentHandler: relation type must be PARENT");
    }
}
