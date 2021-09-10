#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB* pkb) { this->pkb = pkb; }

vector<string> QueryEvaluator::evaluateQuery(Query query) {
    try {
        Reference *returnReference = query.getReturnReference();
        vector<Reference *> references = query.getReferences();
        vector<Relation *> relationships = query.getRelations();
        vector<vector<string>> results(references.size(), vector<string>());
        vector<bool> referenceAppearInClauses(references.size(), false);
        bool allQueryReturnsTrue = true;

        for (Relation *relationship : relationships) {
            Result tempResult;

            RelationshipHandler *relationshipHandler;

            // TODO: add more handlers for relationshipType later
            if (relationship->getType() == RelationType::FOLLOWS) {
                FollowsHandler followsHandler(relationship, pkb);
                relationshipHandler = &followsHandler;
            }

            if (relationship->getType() == RelationType::FOLLOWS_T) {
                FollowsStarHandler followsStarHandler(relationship, pkb);
                relationshipHandler = &followsStarHandler;
            }

            if (relationship->getType() == RelationType::PARENT) {
                ParentHandler parentHandler(relationship, pkb);
                relationshipHandler = &parentHandler;
            }

            if (relationship->getType() == RelationType::PARENT_T) {
                ParentStarHandler parentStarHandler(relationship, pkb);
                relationshipHandler = &parentStarHandler;
            }

            if (relationship->getType() == RelationType::MODIFIES_P) {
                ModifiesProcHandler modifiesProcHandler(relationship, pkb);
                relationshipHandler = &modifiesProcHandler;
            }

            if (relationship->getType() == RelationType::MODIFIES_S) {
                ModifiesStmtHandler modifiesStmtHandler(relationship, pkb);
                relationshipHandler = &modifiesStmtHandler;
            }

            if (relationship->getType() == RelationType::USES_P) {
                UsesProcHandler usesProcHandler(relationship, pkb);
                relationshipHandler = &usesProcHandler;
            }

            if (relationship->getType() == RelationType::USES_S) {
                UsesStmtHandler usesStmtHandler(relationship, pkb);
                relationshipHandler = &usesStmtHandler;
            }

            // eval and combine result
            tempResult = relationshipHandler->eval();
            allQueryReturnsTrue =
                allQueryReturnsTrue && tempResult.isResultValid();

            if (tempResult.hasResultList1()) {
                combineResult(results, references, tempResult.getResultList1(),
                              tempResult.getReference1(),
                              referenceAppearInClauses);
            }

            if (tempResult.hasResultList2()) {
                combineResult(results, references, tempResult.getResultList2(),
                              tempResult.getReference2(),
                              referenceAppearInClauses);
            }
        }

        // returns empty result if one of the boolean clauses returns false
        if (!allQueryReturnsTrue) {
            return vector<string>();
        }

        // returns empty result if one of the references has no matching result
        for (int i = 0; i < references.size(); i++) {
            if (referenceAppearInClauses[i] && results[i].empty()) {
                return vector<string>();
            }
        }

        int resultIndex = -1;
        for (int i = 0; i < references.size(); i++) {
            if (references[i]->equals(*returnReference)) {
                resultIndex = i;
            }
        }

        if (!referenceAppearInClauses[resultIndex]) {
            vector<string> result;

            if (returnReference->getDeType() == DesignEntityType::PROCEDURE) {
                result = pkb->getAllProcs().asVector();
                return result;
            }
            if (returnReference->getDeType() == DesignEntityType::VARIABLE) {
                result = pkb->getAllVars().asVector();
                return result;
            }
            if (returnReference->getDeType() == DesignEntityType::CONSTANT) {
                toString(pkb->getAllConsts().asVector(), result);
                return result;
            }
            if (returnReference->getDeType() == DesignEntityType::ASSIGN) {
                toString(pkb->getAllStmts(StatementType::ASSIGN).asVector(),
                         result);
                return result;
            }
            if (returnReference->getDeType() == DesignEntityType::CALL) {
                toString(pkb->getAllStmts(StatementType::CALL).asVector(),
                         result);
                return result;
            }
            if (returnReference->getDeType() == DesignEntityType::IF) {
                toString(pkb->getAllStmts(StatementType::IF).asVector(),
                         result);
                return result;
            }
            if (returnReference->getDeType() == DesignEntityType::PRINT) {
                toString(pkb->getAllStmts(StatementType::PRINT).asVector(),
                         result);
                return result;
            }
            if (returnReference->getDeType() == DesignEntityType::READ) {
                toString(pkb->getAllStmts(StatementType::READ).asVector(),
                         result);
                return result;
            }
            toString(pkb->getAllStmts().asVector(), result);

            return result;
        }

        return results[resultIndex];
    } catch (RelationHandlerError &e) {
        // to be implemented later
    }
}

void QueryEvaluator::combineResult(vector<vector<string>> &results, vector<Reference *> &references,
                                   vector<string> result, Reference* reference, vector<bool> &referenceAppearInClauses) {
    int index = -1;
    for (int i = 0; i < references.size(); i++) {
        if (references[i]->equals(*reference)) {
            index = i;
        }
    }

    if (referenceAppearInClauses[index] == false) {
        results[index] = result;
        referenceAppearInClauses[index] = true;
    } else {
        vector<string> filteredResult;
        for (string element : results[index]) {
            if (find(result.begin(), result.end(), element) != result.end()) {
                filteredResult.push_back(element);
            }
        }
        results[index] = filteredResult;
    }
}

void QueryEvaluator::toString(vector<int> &vectorIn, vector<string> &vectorOut) {
    transform(vectorIn.begin(), vectorIn.end(), back_inserter(vectorOut),
              [](int i) { return std::to_string(i); });
}
