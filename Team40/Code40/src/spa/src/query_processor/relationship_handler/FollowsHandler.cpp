#include "FollowsHandler.h"

Result FollowsHandler::eval() {
	Result result;
	Entity firstEntity = relationship->getFirstEntity();
	Entity secondEntity = relationship->getSecondEntity();
	string firstStmt = firstEntity.getName();
	string secondStmt = secondEntity.getName();
	bool isFirstEntitySynonym = firstEntity.isSynonymEntity();
	bool isSecondEntitySynonym = secondEntity.isSynonymEntity();

	// Todo later: assert firstEntiy and secondEntity are stmts

	if (!isFirstEntitySynonym &&  !isSecondEntitySynonym) {
		bool queryIsTrue = pkb.getNextStmt(firstStmt) == secondStmt;
		result.setValid(queryIsTrue);
	}

	if (isFirstEntitySynonym && !isSecondEntitySynonym) {
		vector<string> firstStmtResults;
		firstStmtResults.push_back(to_string(pkb.getPrevStmt(secondStmt)));
		result.setResultList1(firstStmtResults);
	}

	if (!isFirstEntitySynonym && isSecondEntitySynonym) {
		vector<string> secondStmtResults;
		secondStmtResults.push_back(to_string(pkb.getNextStmt(firstStmt)));
		result.setResultList2(secondStmtResults);
	}

	if (isFirstEntitySynonym && isSecondEntitySynonym) {
		vector<string> firstStmtResults;
		vector<string> secondStmtResults;
		vector<int> prevStmts = pkb.getAllStmts();
		for (int prevStmt : prevStmts) {
			int nextStmt = pkb.getNextStmt(prevStmt);
			if (nextStmt != -1) { // change accordingly to the PKB API later
				firstStmtResults.push_back(to_string(prevStmt));
				secondStmtResults.push_back(to_string(nextStmt));
			}
		}
		result.setResultList1(firstStmtResults);
		result.setResultList2(secondStmtResults);
	}

	return result;
}
