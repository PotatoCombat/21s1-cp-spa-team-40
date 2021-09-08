#include "FollowsHandler.h"

Result FollowsHandler::eval() {
    Result result;
	Reference* firstReference = relation->getFirstReference();
	Reference* secondReference = relation->getSecondReference();
	string firstStmt = firstReference->getValue();
	string secondStmt = secondReference->getValue();

	// Todo later: assert firstEntiy and secondReference are stmts
	// Todo: use variable instead of magic number

	// WILDCARD WILDCARD
	if (firstReference->getRefType() == ReferenceType::WILDCARD &&
        secondReference->getRefType() == ReferenceType::WILDCARD) {
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
        result.setValid(pkb->getFollowingStmt(stoi(firstStmt)) != -1);
        return result;
	}

	// WILDCARD CONSTANT
    if (firstReference->getRefType() == ReferenceType::WILDCARD &&
        secondReference->getRefType() == ReferenceType::CONSTANT) {
        result.setValid(pkb->getPrecedingStmt(stoi(secondStmt) != -1));
        return result;
	}

	// SYNONYM CONSTANT
	if (firstReference->getRefType() == ReferenceType::SYNONYM && 
		secondReference->getRefType() == ReferenceType::CONSTANT) {
		vector<string> firstStmtResults;
        int precedingStmt = pkb->getPrecedingStmt(stoi(secondStmt));
        if (precedingStmt != -1) {
            firstStmtResults.push_back(to_string(precedingStmt));
        }
		result.setResultList1(firstReference,firstStmtResults);
        return result;
	}

	// CONSTANT SYNONYM
	if (firstReference->getRefType() == ReferenceType::CONSTANT 
		&& secondReference->getRefType() == ReferenceType::SYNONYM) {
		vector<string> secondStmtResults;
        int followingStmt = pkb->getFollowingStmt(stoi(firstStmt));
        if (followingStmt != -1) {
            secondStmtResults.push_back(to_string(followingStmt));
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
