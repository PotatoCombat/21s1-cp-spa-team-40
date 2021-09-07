#include "FollowsHandler.h"

Result FollowsHandler::eval() {
    Result result;
	Entity* firstEntity = relationship->getFirstEntity();
	Entity* secondEntity = relationship->getSecondEntity();
	string firstStmt = firstEntity->getName();
	string secondStmt = secondEntity->getName();
	bool isFirstEntitySynonym = firstEntity->isSynonymEntity();
	bool isSecondEntitySynonym = secondEntity->isSynonymEntity();

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
		!(firstEntity->getName() == Entity::WILDCARD && secondEntity->getName() == Entity::WILDCARD)) {
		vector<string> firstStmtResults;
		vector<string> secondStmtResults;
		vector<int> precedingStmts = pkb->getAllStmts().asVector();
		for (int precedingStmt : precedingStmts) {
			int followingStmt = pkb->getFollowingStmt(precedingStmt);
			if (followingStmt == -1) { // Todo: use variable instead of magic number
				continue;   
			}
			if (firstEntity->getName() != Entity::WILDCARD) {
				firstStmtResults.push_back(to_string(precedingStmt));
            }
			if (secondEntity->getName() != Entity::WILDCARD) {
                secondStmtResults.push_back(to_string(followingStmt));
            }
		}
		result.setResultList1(firstEntity, firstStmtResults);
		result.setResultList2(secondEntity, secondStmtResults);
	}
}
