#include "FollowsHandler.h"

Result FollowsHandler::eval() {
    Result result;
	Reference* firstEntity = relation->getFirstReference();
	Reference* secondEntity = relation->getSecondReference();
	string firstStmt = firstEntity->getValue();
	string secondStmt = secondEntity->getValue();
	bool isFirstEntitySynonym = true;//firstEntity->isSynonym();
	bool isSecondEntitySynonym = true;//secondEntity->isSynonym();

	// Todo later: assert firstEntiy and secondEntity are stmts

	if (!isFirstEntitySynonym &&  !isSecondEntitySynonym) {
		result.setValid(pkb->follows(stoi(firstStmt), stoi(secondStmt)));
	}
        
	if (isFirstEntitySynonym && !isSecondEntitySynonym) {
		vector<string> firstStmtResults;
        int precedingStmt = pkb->getPrecedingStmt(stoi(secondStmt));
        if (precedingStmt != -1) { // Todo: use variable instead of magic number
            firstStmtResults.push_back(to_string(precedingStmt));
        }
		result.setResultList1(firstEntity,firstStmtResults);
	}

	if (!isFirstEntitySynonym && isSecondEntitySynonym) {
		vector<string> secondStmtResults;
        int followingStmt = pkb->getFollowingStmt(stoi(firstStmt));
        if (followingStmt != -1) { // Todo: use variable instead of magic number
            secondStmtResults.push_back(to_string(followingStmt));
		}
		result.setResultList2(secondEntity, secondStmtResults);
	}

	if (isFirstEntitySynonym && isSecondEntitySynonym &&
		!(firstEntity->getValue() == Reference::WILDCARD && secondEntity->getValue() == Reference::WILDCARD)) {
		vector<string> firstStmtResults;
		vector<string> secondStmtResults;
		vector<int> precedingStmts = pkb->getAllStmts().asVector();
		for (int precedingStmt : precedingStmts) {
			int followingStmt = pkb->getFollowingStmt(precedingStmt);
			if (followingStmt == -1) { // Todo: use variable instead of magic number
				continue;   
			}
			if (firstEntity->getValue() != Reference::WILDCARD) {
				firstStmtResults.push_back(to_string(precedingStmt));
            }
			if (secondEntity->getValue() != Reference::WILDCARD) {
                secondStmtResults.push_back(to_string(followingStmt));
            }
		}
		result.setResultList1(firstEntity, firstStmtResults);
		result.setResultList2(secondEntity, secondStmtResults);
	}

	return result;
}
