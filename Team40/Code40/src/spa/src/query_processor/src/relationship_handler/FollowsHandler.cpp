#include "FollowsHandler.h"

Result FollowsHandler::eval() {
    Result result;
	Reference* firstReference = relation->getFirstReference();
	Reference* secondReference = relation->getSecondReference();
	string firstStmt = firstReference->getValue();
	string secondStmt = secondReference->getValue();

    // Todo: handle stmts by different design enity types
	// Todo: use variable instead of magic number -1

    // assertions
    if (firstReference->getDeType() == DesignEntityType::PROCEDURE ||
        firstReference->getDeType() == DesignEntityType::VARIABLE) {
        throw RelationHandlerError("FollowsHandler: first argument must be statement type");
    }

    if (secondReference->getDeType() == DesignEntityType::PROCEDURE ||
        secondReference->getDeType() == DesignEntityType::VARIABLE) {
        throw RelationHandlerError("FollowsHandler: second argument must be statement type");
    }

    if (relation->getType() != RelationType::FOLLOWS) {
        throw RelationHandlerError("FollowsHandler: relation type must be FOLLOWS");
    }

    StatementType firstStmtType = desTypeToStmtType(firstReference->getDeType());
    StatementType secondStmtType = desTypeToStmtType(secondReference->getDeType());

	// WILDCARD WILDCARD
	if (firstReference->getRefType() == ReferenceType::WILDCARD &&
        secondReference->getRefType() == ReferenceType::WILDCARD) {
        vector<int> allStmts = pkb->getAllStmts().asVector();
        
        for (auto stmt : allStmts) {
            int followingStmt = pkb->getFollowingStmt(stmt);
            if (followingStmt != -1) {
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
