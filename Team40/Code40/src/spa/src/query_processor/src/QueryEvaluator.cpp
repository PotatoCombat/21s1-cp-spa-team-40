#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB* pkb) { this->pkb = pkb; }

vector<string> QueryEvaluator::evaluateQuery(Query query) {
    Reference* returnEntity = query.getReturnReference();
    vector<Reference *> entities = query.getReferences();
    vector<Relation *> relationships = query.getRelations();
    vector<vector<string>> results(entities.size(), vector<string>());
    vector<bool> referenceAppearInClauses(entities.size(), false);
    bool allQueryReturnsTrue = true;

    for (Relation *relationship : relationships) {
        Result tempResult;

        RelationshipHandler *relationshipHandler;

        // TODO: add more handlers for relationshipType later
        if (relationship->getType() == RelationType::FOLLOWS) {
            FollowsHandler followsHandler(relationship, pkb);
            relationshipHandler = &followsHandler;
        }

        // eval and combine result
        tempResult = relationshipHandler->eval();
        allQueryReturnsTrue = allQueryReturnsTrue && tempResult.isResultValid();

        if (tempResult.hasResultList1()) {
            combineResult(results, entities, tempResult.getResultList1(),
                          tempResult.getReference1(), referenceAppearInClauses);
        }

        if (tempResult.hasResultList2()) {
            combineResult(results, entities, tempResult.getResultList2(),
                          tempResult.getReference2(), referenceAppearInClauses);
        }
    }

    // returns empty result if one of the boolean clauses returns false
    if (!allQueryReturnsTrue) {
        return vector<string>();
    }

    // returns empty result if one of the references has no matching result
    for (int i = 0; i < entities.size(); i++) {
        if (referenceAppearInClauses[i] && results[i].empty()) {
            return vector<string>();
        }
    }

    int resultIndex = -1;
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->equals(*returnEntity)) {
            resultIndex = i;
        }
    }

    if (!referenceAppearInClauses[resultIndex]) {
        vector<string> result;
        toString(pkb->getAllStmts().asVector(), result); 
        return result;
    }

    return results[resultIndex];
}

void QueryEvaluator::combineResult(vector<vector<string>> &results, vector<Reference *> &references,
                                   vector<string> result, Reference* reference, vector<bool> &entitiesAppearInQuery) {
    int index = -1;
    for (int i = 0; i < references.size(); i++) {
        if (references[i]->equals(*reference)) {
            index = i;
        }
    }

    if (entitiesAppearInQuery[index] == false) {
        results[index] = result;
        entitiesAppearInQuery[index] = true;
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
