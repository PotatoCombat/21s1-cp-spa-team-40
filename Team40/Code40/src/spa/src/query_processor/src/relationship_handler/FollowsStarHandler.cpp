#include "query_processor/relationship_handler/FollowsStarHandler.h"

Result FollowsStarHandler::eval(int ref1Index, int ref2Index) {
    Result result;
     Reference *firstReference = clause->getFirstReference();
     Reference *secondReference = clause->getSecondReference();
     string firstValue = firstReference->getValue();
     string secondValue = secondReference->getValue();

    // Todo: use variable instead of magic number -1

    // assertions
     validate();

    // Follows*(s, s)
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
            if (pkb->getFollowingStmt(stmt) != -1) {
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
        result.setValid(pkb->followsStar(stoi(firstValue),
        stoi(secondValue))); return result;
    }

    // CONSTANT WILDCARD
     if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::WILDCARD) {
        result.setValid(pkb->getFollowingStmt(stoi(firstValue)) != -1);
        return result;
    }

    // WILDCARD CONSTANT
     if (firstReference->getRefType() == ReferenceType::WILDCARD &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        result.setValid(pkb->getPrecedingStmt(stoi(secondValue)) != -1);
        return result;
    }

    // SYNONYM CONSTANT
     if (firstReference->getRefType() == ReferenceType::SYNONYM &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        vector<ValueToPointersMap> firstStmtResults;
        set<int> precedingStarStmts =
        pkb->getPrecedingStarStmts(stoi(secondValue)); for (auto
        precedingStarStmt : precedingStarStmts) {
            if (isDesTypeStmtType(firstReference->getDeType(),
            pkb->getStmtType(precedingStarStmt))) {
                ValueToPointersMap map(to_string(precedingStarStmt),
                                       POINTER_SET{});
                firstStmtResults.push_back(map);
            }
        }
        result.setResultList1(firstReference, firstStmtResults);
        return result;
    }

    // CONSTANT SYNONYM
     if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::SYNONYM) {
        vector<ValueToPointersMap> secondStmtResults;
        set<int> followingStarStmts =
        pkb->getFollowingStarStmts(stoi(firstValue)); for (auto
        followingStarStmt : followingStarStmts) {
            if (isDesTypeStmtType(secondReference->getDeType(),
            pkb->getStmtType(followingStarStmt))) {
                ValueToPointersMap map(to_string(followingStarStmt),
                                       POINTER_SET{});
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
        vector<int> precedingStmts;
        if (firstReference->getDeType() == DesignEntityType::STMT) {
            precedingStmts = pkb->getAllStmts().asVector();
        } else {
            StatementType firstStmtType =
                desTypeToStmtType(firstReference->getDeType());
            precedingStmts = pkb->getAllStmts(firstStmtType).asVector();
        }

        for (int precedingStmt : precedingStmts) {
            set<int> followingStmts =
            pkb->getFollowingStarStmts(precedingStmt); POINTER_SET related;
            bool valid = false;
            for (auto stmt : followingStmts) {
                if (isDesTypeStmtType(secondReference->getDeType(),
                                      pkb->getStmtType(stmt))) {
                    valid = true;
                    if (secondReference->getRefType() == ReferenceType::SYNONYM) {
                        related.insert(make_pair(ref2Index, to_string(stmt)));
                    }
                }
            }
            if (valid) {
                ValueToPointersMap map(to_string(precedingStmt), related);
                firstStmtResults.push_back(map);
            }
        }

        result.setResultList1(firstReference, firstStmtResults);
    }

    // if second arg is SYNONYM
     if (secondReference->getRefType() != ReferenceType::WILDCARD) {
        vector<ValueToPointersMap> secondStmtResults;
        vector<int> followingStmts;
        if (secondReference->getDeType() == DesignEntityType::STMT) {
            followingStmts = pkb->getAllStmts().asVector();
        } else {
            StatementType secondStmtType =
                desTypeToStmtType(secondReference->getDeType());
            followingStmts = pkb->getAllStmts(secondStmtType).asVector();
        }

        for (int followingStmt : followingStmts) {
            set<int> precedingStmts =
            pkb->getPrecedingStarStmts(followingStmt); POINTER_SET related;
            bool valid = false;
            for (auto stmt : precedingStmts) {
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
                ValueToPointersMap map(to_string(followingStmt), related);
                secondStmtResults.push_back(map);
            }
        }

        result.setResultList2(secondReference, secondStmtResults);
    }

    return result;
}

void FollowsStarHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "FollowsStarHandler: first argument must be statement type");
    }

    if (secondReference->getDeType() == DesignEntityType::PROCEDURE ||
        secondReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "FollowsStarHandler: second argument must be statement type");
    }

    if (clause->getType() != ClauseType::FOLLOWS_T) {
        throw ClauseHandlerError(
            "FollowsStarHandler: relation type must be FOLLOWS_T");
    }
}
