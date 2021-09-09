#include "ParentStarHandler.h"

Result ParentStarHandler::eval() {
    Result result;
    Reference *firstReference = relation->getFirstReference();
    Reference *secondReference = relation->getSecondReference();
    string firstStmt = firstReference->getValue();
    string secondStmt = secondReference->getValue();

    // Todo: handle stmts by different design enity types
    // Todo: assert relationType is follows
    // Todo: assert firstEntiy and secondReference are stmts
    // Todo: use variable instead of magic number -1

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
        vector<string> firstStmtResults;
        set<int> parentStarStmts = pkb->getParentStarStmts(stoi(secondStmt));
        for (auto parentStarStmt : parentStarStmts) {
            firstStmtResults.push_back(to_string(parentStarStmt));
        }
        result.setResultList1(firstReference, firstStmtResults);
        return result;
    }

    // CONSTANT SYNONYM
    if (firstReference->getRefType() == ReferenceType::CONSTANT &&
        secondReference->getRefType() == ReferenceType::SYNONYM) {
        vector<string> secondStmtResults;
        set<int> childStarStmts = pkb->getChildStarStmts(stoi(firstStmt));
        for (auto childStarStmt : childStarStmts) {
            secondStmtResults.push_back(to_string(childStarStmt));
        }
        result.setResultList2(secondReference, secondStmtResults);
        return result;
    }

   // NEITHER IS CONSTANT
    vector<string> firstStmtResults;
    vector<string> secondStmtResults;
    vector<int> parentStmts = pkb->getAllStmts().asVector();
    for (int parentStmt : parentStmts) {
        set<int> childStmts = pkb->getChildStmts(parentStmt);
        if (childStmts.size() == 0) {
            continue;
        }
        firstStmtResults.push_back(to_string(parentStmt));
        for (auto childStmt : childStmts) {
            secondStmtResults.push_back(to_string(childStmt));
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
