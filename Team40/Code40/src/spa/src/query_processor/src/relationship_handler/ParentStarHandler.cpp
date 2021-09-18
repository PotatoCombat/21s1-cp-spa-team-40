#include "query_processor/relationship_handler/ParentStarHandler.h"

Result ParentStarHandler::eval() {
    Result result;
   // Reference *firstReference = clause->getFirstReference();
   // Reference *secondReference = clause->getSecondReference();
   // string firstStmt = firstReference->getValue();
   // string secondStmt = secondReference->getValue();

   // // Todo: use variable instead of magic number -1

   // // assertions
   // validate();

   // // Parent*(s, s)
   // if (firstReference->getRefType() == ReferenceType::SYNONYM &&
   //     firstReference->equals(*secondReference)) {
   //     result.setValid(false);
   //     return result;
   // }

   // // WILDCARD WILDCARD
   // if (firstReference->getRefType() == ReferenceType::WILDCARD &&
   //     secondReference->getRefType() == ReferenceType::WILDCARD) {
   //     vector<int> allStmts = pkb->getAllStmts().asVector();
   //     for (auto stmt : allStmts) {
   //         if (pkb->getParentStmt(stmt) != -1) {
   //             result.setValid(true);
   //             return result;
   //         }
   //     }
   //     result.setValid(false);
   //     return result;
   // }

   // /// CONSTANT CONSTANT
   // if (firstReference->getRefType() == ReferenceType::CONSTANT &&
   //     secondReference->getRefType() == ReferenceType::CONSTANT) {
   //     result.setValid(pkb->parentStar(stoi(firstStmt), stoi(secondStmt)));
   //     return result;
   // }

   // // CONSTANT WILDCARD
   // if (firstReference->getRefType() == ReferenceType::CONSTANT &&
   //     secondReference->getRefType() == ReferenceType::WILDCARD) {
   //     result.setValid(pkb->getChildStmts(stoi(firstStmt)).size() > 0);
   //     return result;
   // }

   // // WILDCARD CONSTANT
   // if (firstReference->getRefType() == ReferenceType::WILDCARD &&
   //     secondReference->getRefType() == ReferenceType::CONSTANT) {
   //     result.setValid(pkb->getParentStmt(stoi(secondStmt)) != -1);
   //     return result;
   // }

   // // SYNONYM CONSTANT
   // if (firstReference->getRefType() == ReferenceType::SYNONYM &&
   //     secondReference->getRefType() == ReferenceType::CONSTANT) {
   //     vector<string> firstStmtResults;
   //     set<int> parentStarStmts = pkb->getParentStarStmts(stoi(secondStmt));
   //     for (auto parentStarStmt : parentStarStmts) {
   //         if (isDesTypeStmtType(firstReference->getDeType(), pkb->getStmtType(parentStarStmt))) {
   //             firstStmtResults.push_back(to_string(parentStarStmt));
   //         }
   //     }
   //     result.setResultList1(firstReference, firstStmtResults);
   //     return result;
   // }

   // // CONSTANT SYNONYM
   // if (firstReference->getRefType() == ReferenceType::CONSTANT &&
   //     secondReference->getRefType() == ReferenceType::SYNONYM) {
   //     vector<string> secondStmtResults;
   //     set<int> childStarStmts = pkb->getChildStarStmts(stoi(firstStmt));
   //     for (auto childStarStmt : childStarStmts) {
   //         if (isDesTypeStmtType(secondReference->getDeType(), pkb->getStmtType(childStarStmt))) {
   //             secondStmtResults.push_back(to_string(childStarStmt));
   //         }
   //     }
   //     result.setResultList2(secondReference, secondStmtResults);
   //     return result;
   // }

   //// NEITHER IS CONSTANT
   // vector<string> firstStmtResults;
   // vector<string> secondStmtResults;
   // vector<int> childStmts;
   // if (secondReference->getDeType() == DesignEntityType::STMT) {
   //     childStmts = pkb->getAllStmts().asVector();
   // } else {
   //     StatementType secondStmtType = desTypeToStmtType(secondReference->getDeType());
   //     childStmts = pkb->getAllStmts(secondStmtType).asVector();
   // }
   // sort(childStmts.begin(), childStmts.end(), greater<int>());

   // while (childStmts.size() > 0) {
   //     int childStmt = childStmts.at(0);
   //     vector<int> tempStmts;
   //     tempStmts.push_back(childStmt);
   //     bool hasParentStar = false;
   //     int currStmt = pkb->getParentStmt(childStmt);
   //     while (currStmt != -1) {
   //         StatementType stmtType = pkb->getStmtType(currStmt);
   //         if (isDesTypeStmtType(firstReference->getDeType(), stmtType)) {
   //             /*if (find(firstStmtResults.begin(), firstStmtResults.end(), to_string(currStmt)) != 
   //                 firstStmtResults.end()) {
   //                 secondStmtResults.push_back(to_string(childStmt));
   //                 hasParentStar = true;
   //                 childStmts.erase(
   //                     remove(childStmts.begin(), childStmts.end(), childStmt),
   //                     childStmts.end());
   //             } else {*/
   //             for (auto stmt : tempStmts) {
   //                 secondStmtResults.push_back(to_string(stmt));
   //                 childStmts.erase(
   //                     remove(childStmts.begin(), childStmts.end(), stmt),
   //                     childStmts.end());
   //             }
   //             tempStmts.clear();
   //             hasParentStar = true;
   //             if (find(firstStmtResults.begin(), firstStmtResults.end(), to_string(currStmt)) != 
   //                 firstStmtResults.end()) {
   //                 // if currStmt is in firstStmtResult already, it means the parents of it has been considered before
   //                 break;
   //             }
   //             firstStmtResults.push_back(to_string(currStmt));
   //         }
   //         if (isDesTypeStmtType(secondReference->getDeType(), stmtType)) {
   //             tempStmts.push_back(currStmt);
   //         }

   //         currStmt = pkb->getParentStmt(currStmt);


   //     }

   //     // still removed from the precedingStmts when don't have followsStar
   //     if (!hasParentStar) {
   //         childStmts.erase(remove(childStmts.begin(),childStmts.end(), childStmt),
   //                              childStmts.end());
   //     }
   // }

   // if (firstReference->getRefType() != ReferenceType::WILDCARD) {
   //     result.setResultList1(firstReference, firstStmtResults);
   // }

   // if (secondReference->getRefType() != ReferenceType::WILDCARD) {
   //     result.setResultList2(secondReference, secondStmtResults);
   // }

    return result;
}

void ParentStarHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("ParentStarHandler: first argument must be statement type");
    }

    if (secondReference->getDeType() == DesignEntityType::PROCEDURE ||
        secondReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("ParentStarHandler: second argument must be statement type");
    }

    if (clause->getType() != ClauseType::PARENT_T) {
        throw ClauseHandlerError("ParentStarHandler: relation type must be PARENT_T");
    }
}
