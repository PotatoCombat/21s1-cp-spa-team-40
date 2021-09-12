#include "FollowsStarHandler.h"

Result FollowsStarHandler::eval() {
    Result result;
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    string firstValue = firstReference->getValue();
    string secondValue = secondReference->getValue();

    // Todo: handle stmts by different design enity types
    // Todo: use variable instead of magic number -1

    // assertions
    validate();

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
            if (isDesTypeStmtType(firstReference->getDeType(), pkb->getStmtType(precedingStarStmt))) {
                firstStmtResults.push_back(to_string(precedingStarStmt));
            }
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
            if (isDesTypeStmtType(secondReference->getDeType(), pkb->getStmtType(followingStarStmt))) {
                secondStmtResults.push_back(to_string(followingStarStmt));
            }
        }
        result.setResultList2(secondReference, secondStmtResults);
        return result;
    }

    // NEITHER IS CONSTANT
    vector<string> firstStmtResults;
    vector<string> secondStmtResults;

    vector<int> precedingStarStmts;
    if (firstReference->getDeType() == DesignEntityType::STMT) {
        precedingStarStmts = pkb->getAllStmts().asVector();
    } else {
        StatementType firstStmtType = desTypeToStmtType(firstReference->getDeType());
        precedingStarStmts = pkb->getAllStmts(firstStmtType).asVector();
    }

    while (precedingStarStmts.size() > 0) {
        int precedingStarStmt = precedingStarStmts.at(0);
        vector<int> tempStmts;
        tempStmts.push_back(precedingStarStmt);
        bool hasFollowsStar = false;
        set<int> followingStarStmts = pkb->getFollowingStarStmts(precedingStarStmt);
        for (int followingStarStmt : followingStarStmts) {
            StatementType stmtType = pkb->getStmtType(followingStarStmt);
             if (isDesTypeStmtType(secondReference->getDeType(), stmtType)) {
                for (auto stmt : tempStmts) {
                    firstStmtResults.push_back(to_string(stmt));
                    precedingStarStmts.erase(
                        remove(precedingStarStmts.begin(), precedingStarStmts.end(), stmt), 
                        precedingStarStmts.end());
                }
                tempStmts.clear();
                secondStmtResults.push_back(to_string(followingStarStmt));
                hasFollowsStar = true;
             }
             if (isDesTypeStmtType(firstReference->getDeType(), stmtType)) {
                tempStmts.push_back(followingStarStmt);
             }
        }

        // still removed from the precedingStarStmts when don't have followsStar
        precedingStarStmts.erase(
            remove(precedingStarStmts.begin(), precedingStarStmts.end(), precedingStarStmt),
            precedingStarStmts.end());
    
    }
    
    /*vector<int> precedingStarStmts;
    if (firstReference->getDeType() == DesignEntityType::STMT) {
        precedingStarStmts = pkb->getAllStmts().asVector();
    } else {
        StatementType firstStmtType = desTypeToStmtType(firstReference->getDeType());
        precedingStarStmts = pkb->getAllStmts(firstStmtType).asVector();
    }

    for (int precedingStarStmt : precedingStarStmts) {
        set<int> followingStarStmts = pkb->getFollowingStarStmts(precedingStarStmt);

        int followingStmt = pkb->getFollowingStmt(precedingStarStmt);
        if (followingStmt == -1 ||
            !isDesTypeStmtType(secondReference->getDeType(), pkb->getStmtType(followingStmt))) {
            continue;
        }


        firstStmtResults.push_back(to_string(precedingStarStmt));
        secondStmtResults.push_back(to_string(followingStmt));
    }*/

    if (firstReference->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList1(firstReference, firstStmtResults);
    }

    if (secondReference->getRefType() != ReferenceType::WILDCARD) {
        result.setResultList2(secondReference, secondStmtResults);
    }

    return result;
}

void FollowsStarHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("FollowsStarHandler: first argument must be statement type");
    }

    if (secondReference->getDeType() == DesignEntityType::PROCEDURE ||
        secondReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("FollowsStarHandler: second argument must be statement type");
    }

    if (clause->getType() != ClauseType::FOLLOWS_T) {
        throw ClauseHandlerError("FollowsStarHandler: relation type must be FOLLOWS_T");
    }
}
