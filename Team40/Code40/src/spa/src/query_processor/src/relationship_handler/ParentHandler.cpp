#include "query_processor/relationship_handler/ParentHandler.h"

Result ParentHandler::eval(int ref1Index, int ref2Index) {
    Result result;
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    string firstStmt = firstReference->getValue();
    string secondStmt = secondReference->getValue();

    // Todo: handle stmts by different design enity types
    // Todo: use variable instead of magic number -1

    // assertions
    validate();

    // Parent(s, s)
    if (firstReference->getRefType() == ReferenceType::SYNONYM &&
        firstReference->equals(*secondReference)) {
        result.setValid(false);
        return result;
    }

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
        vector<ValueToPointersMap> firstStmtResults;
        int parentStmt = pkb->getParentStmt(stoi(secondStmt));
        if (parentStmt != -1 &&
            isDesTypeStmtType(firstReference->getDeType(),
                              pkb->getStmtType(parentStmt))) {
            ValueToPointersMap map(to_string(parentStmt), POINTER_SET{});
            firstStmtResults.push_back(map);
        }
        result.setResultList1(firstReference, firstStmtResults);
        return result;
    }

    // CONSTANT SYNONYM
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::SYNONYM) {
        vector<ValueToPointersMap> secondStmtResults;
        set<int> childStmts = pkb->getChildStmts(stoi(firstStmt));
        for (auto childStmt : childStmts) {
            if (isDesTypeStmtType(secondReference->getDeType(),
                                  pkb->getStmtType(childStmt))) {
                ValueToPointersMap map(to_string(childStmt), POINTER_SET{});
                secondStmtResults.push_back(map);
            }
        }
        result.setResultList2(secondReference, secondStmtResults);
        return result;
    }

    // NEITHER IS CONSTANT
    // if first arg is SYNONYM
    if (firstReference->getRefType() != ReferenceType::WILDCARD) {
        vector<ValueToPointersMap> firstStmtResults;
        vector<int> parentStmts;
        if (firstReference->getDeType() == DesignEntityType::STMT) {
            parentStmts = pkb->getAllStmts().asVector();
        } else {
            StatementType firstStmtType =
                desTypeToStmtType(firstReference->getDeType());
            parentStmts = pkb->getAllStmts(firstStmtType).asVector();
        }

        for (int parentStmt : parentStmts) {
            set<int> childStmts = pkb->getChildStmts(parentStmt);
            POINTER_SET related;
            bool valid = false;
            for (auto stmt : childStmts) {
                if (isDesTypeStmtType(secondReference->getDeType(),
                                      pkb->getStmtType(stmt))) {
                    valid = true;
                    if (secondReference->getRefType() == ReferenceType::SYNONYM) {
                        related.insert(make_pair(ref2Index, to_string(stmt)));
                    }
                }
            }
            if (valid) {
                ValueToPointersMap map(to_string(parentStmt), related);
                firstStmtResults.push_back(map);
            }
        }

        result.setResultList1(firstReference, firstStmtResults);
    }

    // if second arg is SYNONYM
    if (secondReference->getRefType() != ReferenceType::WILDCARD) {
        vector<ValueToPointersMap> secondStmtResults;
        vector<int> childStmts;
        if (secondReference->getDeType() == DesignEntityType::STMT) {
            childStmts = pkb->getAllStmts().asVector();
        } else {
            StatementType secondStmtType =
                desTypeToStmtType(secondReference->getDeType());
            childStmts = pkb->getAllStmts(secondStmtType).asVector();
        }

        for (int childStmt : childStmts) {
            int parentStmt = pkb->getParentStmt(childStmt);
            if (parentStmt != -1 && isDesTypeStmtType(firstReference->getDeType(),
                                  pkb->getStmtType(parentStmt))) {
                ValueToPointersMap map(to_string(childStmt),
                        POINTER_SET{make_pair(ref1Index, to_string(parentStmt))});
                secondStmtResults.push_back(map);
            }
        }

        result.setResultList2(secondReference, secondStmtResults);
    }

    return result;
}

void ParentHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "ParentHandler: first argument must be statement type");
    }

    if (secondReference->getDeType() == DesignEntityType::PROCEDURE ||
        secondReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "ParentHandler: second argument must be statement type");
    }

    if (clause->getType() != ClauseType::PARENT) {
        throw ClauseHandlerError("ParentHandler: relation type must be PARENT");
    }
}
