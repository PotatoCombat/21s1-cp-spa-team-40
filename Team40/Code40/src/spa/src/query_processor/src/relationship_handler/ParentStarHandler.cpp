#include "query_processor/relationship_handler/ParentStarHandler.h"

Result ParentStarHandler::eval(int ref1Index, int ref2Index) {
    Result result;
     Reference *firstReference = clause->getFirstReference();
     Reference *secondReference = clause->getSecondReference();
     string firstStmt = firstReference->getValue();
     string secondStmt = secondReference->getValue();

    // Todo: use variable instead of magic number -1

    // assertions
     validate();

    // Parent*(s, s)
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
        result.setValid(pkb->parentStar(stoi(firstStmt), stoi(secondStmt)));
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
        set<int> parentStarStmts = pkb->getParentStarStmts(stoi(secondStmt));
        for (auto parentStarStmt : parentStarStmts) {
            if (isDesTypeStmtType(firstReference->getDeType(),
            pkb->getStmtType(parentStarStmt))) {
                ValueToPointersMap map(to_string(parentStarStmt),
                POINTER_SET{}); firstStmtResults.push_back(map);
            }
        }
        result.setResultList1(firstReference, firstStmtResults);
        return result;
    }

    // CONSTANT SYNONYM
     if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::SYNONYM) {
        vector<ValueToPointersMap> secondStmtResults;
        set<int> childStarStmts = pkb->getChildStarStmts(stoi(firstStmt));
        for (auto childStarStmt : childStarStmts) {
            if (isDesTypeStmtType(secondReference->getDeType(),
            pkb->getStmtType(childStarStmt))) {
                ValueToPointersMap map(to_string(childStarStmt),
                POINTER_SET{}); secondStmtResults.push_back(map);
            }
        }
        result.setResultList2(secondReference, secondStmtResults);
        return result;
    }

    // NEITHER IS CONSTANT
    // if first arg is SYNONYM
     if (firstReference->getRefType() != ReferenceType::WILDCARD) {
        vector<ValueToPointersMap> firstStmtResults;
        vector<int> parentStarStmts;
        if (firstReference->getDeType() == DesignEntityType::STMT) {
            parentStarStmts = pkb->getAllStmts().asVector();
        } else {
            StatementType firstStmtType =
                desTypeToStmtType(firstReference->getDeType());
            parentStarStmts = pkb->getAllStmts(firstStmtType).asVector();
        }

        for (int parentStarStmt : parentStarStmts) {
            set<int> childStarStmts = pkb->getChildStarStmts(parentStarStmt);
            POINTER_SET related;
            bool valid = false;
            for (auto stmt : childStarStmts) {
                if (isDesTypeStmtType(secondReference->getDeType(),
                pkb->getStmtType(stmt))) {
                    valid = true;
                    if (secondReference->getRefType() ==
                    ReferenceType::SYNONYM) {
                        related.insert(make_pair(ref2Index, to_string(stmt)));
                    }
                }
            }
            if (valid) {
                ValueToPointersMap map(to_string(parentStarStmt), related);
                firstStmtResults.push_back(map);
            }
        }
        result.setResultList1(firstReference, firstStmtResults);
    }

    // if second arg is SYNONYM
     if (secondReference->getRefType() != ReferenceType::WILDCARD) {
        vector<ValueToPointersMap> secondStmtResults;
        vector<int> childStarStmts;
        if (secondReference->getDeType() == DesignEntityType::STMT) {
            childStarStmts = pkb->getAllStmts().asVector();
        } else {
            StatementType secondStmtType =
                desTypeToStmtType(secondReference->getDeType());
            childStarStmts = pkb->getAllStmts(secondStmtType).asVector();
        }

        for (int childStarStmt : childStarStmts) {
            set<int> parentStarStmts = pkb->getParentStarStmts(childStarStmt);
            POINTER_SET related;
            bool valid = false;
            for (auto stmt : parentStarStmts) {
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
                ValueToPointersMap map(to_string(childStarStmt), related);
                secondStmtResults.push_back(map);
            }
        }

        result.setResultList2(secondReference, secondStmtResults);
    }

    return result;
}

void ParentStarHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "ParentStarHandler: first argument must be statement type");
    }

    if (secondReference->getDeType() == DesignEntityType::PROCEDURE ||
        secondReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "ParentStarHandler: second argument must be statement type");
    }

    if (clause->getType() != ClauseType::PARENT_T) {
        throw ClauseHandlerError(
            "ParentStarHandler: relation type must be PARENT_T");
    }
}
