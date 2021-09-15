#include "FollowsHandler.h"

Result FollowsHandler::eval() {
    Result result;
	Reference* firstReference = clause->getFirstReference();
	Reference* secondReference = clause->getSecondReference();
	string firstStmt = firstReference->getValue();
	string secondStmt = secondReference->getValue();

	// Todo: use variable instead of magic number -1

    // assertions
    validate();

    // Follows(s, s)
    if (firstReference->getRefType() == ReferenceType::WILDCARD &&
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
		vector<string> firstStmtResults;
        int precedingStmt = pkb->getPrecedingStmt(stoi(secondStmt));
        if (precedingStmt != -1 &&
            isDesTypeStmtType(firstReference->getDeType(), pkb->getStmtType(precedingStmt))) {
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
        if (followingStmt != -1 && 
            isDesTypeStmtType(secondReference->getDeType(), pkb->getStmtType(followingStmt))) {
            secondStmtResults.push_back(to_string(followingStmt));
		}
		result.setResultList2(secondReference, secondStmtResults);
		return result;
	}

	// NEITHER IS CONSTANT
    vector<string> firstStmtResults;
    vector<string> secondStmtResults;
    vector<int> precedingStmts;
    if (firstReference->getDeType() == DesignEntityType::STMT) {
        precedingStmts = pkb->getAllStmts().asVector();
    } else {
        StatementType firstStmtType = desTypeToStmtType(firstReference->getDeType());
        precedingStmts = pkb->getAllStmts(firstStmtType).asVector();
    }

    for (int precedingStmt : precedingStmts) {
        int followingStmt = pkb->getFollowingStmt(precedingStmt);
        if (followingStmt == -1 ||
            !isDesTypeStmtType(secondReference->getDeType(), pkb->getStmtType(followingStmt))) {
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

void FollowsHandler::validate() {
    Reference *firstReference = clause->getFirstReference();
    Reference *secondReference = clause->getSecondReference();
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("FollowsHandler: first argument must be statement type");
    }

    if (secondReference->getDeType() == DesignEntityType::PROCEDURE ||
        secondReference->getDeType() == DesignEntityType::VARIABLE) {
        throw ClauseHandlerError("FollowsHandler: second argument must be statement type");
    }

    if (clause->getType() != ClauseType::FOLLOWS) {
        throw ClauseHandlerError("FollowsHandler: relation type must be FOLLOWS");
    }
}
