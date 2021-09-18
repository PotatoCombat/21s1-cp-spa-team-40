#include "query_processor/relationship_handler/FollowsHandler.h"

Result FollowsHandler::eval(int ref1Index, int ref2Index) {
    Result result;
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    string firstStmt = firstReference->getValue();
    string secondStmt = secondReference->getValue();

    // Todo: use variable instead of magic number -1

    // assertions
    validate();

    // Follows(s, s)
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
        result.setValid(pkb->follows(stoi(firstStmt), stoi(secondStmt)));
        return result;
    }

    // CONSTANT WILDCARD
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::WILDCARD) {
        int followingStmt = pkb->getFollowingStmt(stoi(firstStmt));
        result.setValid(followingStmt != -1);
        return result;
    }

    // WILDCARD CONSTANT
    if (firstReference->getRefType() == ReferenceType::WILDCARD &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        int precedingStmt = pkb->getPrecedingStmt(stoi(secondStmt));
        result.setValid(precedingStmt != -1);
        return result;
    }

    // SYNONYM CONSTANT
    if (firstReference->getRefType() == ReferenceType::SYNONYM &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        vector<ValueToPointersMap> firstStmtResults;
        int precedingStmt = pkb->getPrecedingStmt(stoi(secondStmt));
        if (precedingStmt != -1 &&
            isDesTypeStmtType(firstReference->getDeType(),
                              pkb->getStmtType(precedingStmt))) {
            ValueToPointersMap element(to_string(precedingStmt), POINTER_SET{});
            firstStmtResults.push_back(element);
        }
        result.setResultList1(firstReference, firstStmtResults);
        return result;
    }

    // CONSTANT SYNONYM
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::SYNONYM) {
        vector<ValueToPointersMap> secondStmtResults;
        int followingStmt = pkb->getFollowingStmt(stoi(firstStmt));
        if (followingStmt != -1 &&
            isDesTypeStmtType(secondReference->getDeType(),
                              pkb->getStmtType(followingStmt))) {
            ValueToPointersMap element(to_string(followingStmt), POINTER_SET{});
            secondStmtResults.push_back(element);
        }
        result.setResultList2(secondReference, secondStmtResults);
        return result;
    }

    // NEITHER IS CONSTANT
    vector<ValueToPointersMap> firstStmtResults;
    vector<ValueToPointersMap> secondStmtResults;
    vector<int> precedingStmts;
    if (firstReference->getDeType() == DesignEntityType::STMT) {
        precedingStmts = pkb->getAllStmts().asVector();
    } else {
        StatementType firstStmtType =
            desTypeToStmtType(firstReference->getDeType());
        precedingStmts = pkb->getAllStmts(firstStmtType).asVector();
    }

    for (int precedingStmt : precedingStmts) {
        int followingStmt = pkb->getFollowingStmt(precedingStmt);
        if (followingStmt == -1 ||
            !isDesTypeStmtType(secondReference->getDeType(),
                               pkb->getStmtType(followingStmt))) {
            continue;
        }
        if (secondReference->getRefType() != ReferenceType::WILDCARD) {
            ValueToPointersMap element1(
                to_string(precedingStmt),
                POINTER_SET{make_pair(ref2Index, to_string(followingStmt))});

            firstStmtResults.push_back(element1);
        } else {
            ValueToPointersMap element1(to_string(precedingStmt),
                                        POINTER_SET{});

            firstStmtResults.push_back(element1);
        }
        if (firstReference->getRefType() != ReferenceType::WILDCARD) {
            ValueToPointersMap element2(
                to_string(followingStmt),
                POINTER_SET{make_pair(ref1Index, to_string(precedingStmt))});

            secondStmtResults.push_back(element2);
        } else {
            ValueToPointersMap element2(to_string(followingStmt),
                                        POINTER_SET{});

            secondStmtResults.push_back(element2);
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

void FollowsHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "FollowsHandler: first argument must be statement type");
    }

    if (secondReference->getDeType() == DesignEntityType::PROCEDURE ||
        secondReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError(
            "FollowsHandler: second argument must be statement type");
    }

    if (clause->getType() != ClauseType::FOLLOWS) {
        throw ClauseHandlerError(
            "FollowsHandler: relation type must be FOLLOWS");
    }
}
