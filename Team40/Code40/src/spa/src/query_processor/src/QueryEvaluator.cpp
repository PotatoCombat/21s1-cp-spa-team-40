#include "QueryEvaluator.h"

QueryEvaluator::QueryEvaluator(PKB pkb) { this->pkb = pkb; }

vector<string> QueryEvaluator::evaluateQuery(Query query) {
    Reference* returnEntity = query.getReturnReference();
    vector<Reference *> entities = query.getReferences(); // TODO: placeholder for now, need to change
    vector<Relation *> relationships = query.getRelations();
    vector<vector<string>> results(entities.size(), vector<string>());
    vector<bool> entitiesAppearInQuery(entities.size(), false);
    bool allQueryReturnsTrue = true;

    for (vector<Relation *>::iterator it = relationships.begin();
         it != relationships.end(); ++it) {
    
    } 

    for (Relation *relationship : relationships) {
        Result tempResult;

        RelationshipHandler *relationshipHandler;

        // TODO: add more handlers for relationshipType later
        if (relationship->getType() == RelationType::FOLLOWS) {
            FollowsHandler followsHandler(relationship, &pkb);
            relationshipHandler = &followsHandler;
        }

        // eval and combine result
        tempResult = relationshipHandler->eval();
        allQueryReturnsTrue = allQueryReturnsTrue && tempResult.isResultValid();

        if (tempResult.hasResultList1()) {
            combineResult(results, entities, tempResult.getResultList1(),
                          tempResult.getReference1(), entitiesAppearInQuery);
        }

        if (tempResult.hasResultList2()) {
            combineResult(results, entities, tempResult.getResultList2(),
                          tempResult.getReference2(), entitiesAppearInQuery);
        }
    }

    if (!allQueryReturnsTrue) {
        return vector<string>();
    }

    int resultIndex = -1;
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->equals(*returnEntity)) {
            resultIndex = i;
        }
    }

    if (entitiesAppearInQuery[resultIndex] = false) {
        vector<string> result;
        toString(pkb.getAllStmts().asVector(), result); 
        return result;
    }

    return results[resultIndex];
}

void QueryEvaluator::combineResult(vector<vector<string>> &results, vector<Reference *> &entities,
                                   vector<string> result, Reference* entity, vector<bool> &entitiesAppearInQuery) {
    int index = -1;
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->equals(*entity)) {
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
