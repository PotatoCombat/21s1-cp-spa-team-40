#include "FollowsStarHandler.h"

Result FollowsStarHandler::eval() {
    Result result;
    Reference *firstReference = relation->getFirstReference();
    Reference *secondReference = relation->getSecondReference();
    string firstValue = firstReference->getValue();
    string secondValue = secondReference->getValue();

    // Todo: handle stmts by different design enity types
    // Todo: assert relationType is follows
    // Todo: assert firstEntiy and secondReference are stmts
    // Todo: use variable instead of magic number -1

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
        result.setValid(pkb->followsStar(stoi(firstValue), stoi(secondValue)));
        return result;
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
        vector<string> firstStmtResults;
        set<int> precedingStarStmts = pkb->getPrecedingStarStmts(stoi(secondValue));
        for (auto precedingStarStmt : precedingStarStmts) {
            firstStmtResults.push_back(to_string(precedingStarStmt));
        }
        result.setResultList1(firstReference, firstStmtResults);
        return result;
    }

    // CONSTANT SYNONYM
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::SYNONYM) {
        vector<string> secondStmtResults;
        set<int> followingStarStmts = pkb->getFollowingStarStmts(stoi(firstValue));
        for (auto followingStarStmt : followingStarStmts) {
            secondStmtResults.push_back(to_string(followingStarStmt));
        }
        result.setResultList2(secondReference, secondStmtResults);
        return result;
    }

    // NEITHER IS CONSTANT
    vector<string> firstStmtResults;
    vector<string> secondStmtResults;
    vector<int> precedingStmts = pkb->getAllStmts().asVector();
    for (int precedingStmt : precedingStmts) {
        int followingStmt = pkb->getFollowingStmt(precedingStmt);
        if (followingStmt == -1) {
            continue;
        }
        firstStmtResults.push_back(to_string(precedingStmt));
        secondStmtResults.push_back(to_string(followingStmt));
    }

    if (firstReference->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList1(firstReference, firstStmtResults);
    }

    if (secondReference->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList2(secondReference, secondStmtResults);
    }

    return result;
}
