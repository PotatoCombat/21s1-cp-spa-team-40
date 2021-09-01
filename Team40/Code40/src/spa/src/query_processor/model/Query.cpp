#include "Query.h"

using namespace std;

Query::Query() {
    vector<Entity *> entityList;
    vector<Relationship *> relationshipList;
    Entity *returnEntity;
}

Entity Query::GetReturnEntity() { return returnEntity; }

void Query::SetReturnEntity(Entity *entity) { returnEntity = entity; }

void Query::AddEntity(Entity *entity) { entityList.push_back(entity); }

vector<Entity *> Query::GetEntities() { return entityList; }

void Query::AddRelationship(Relationship *relationship) {
    relationshipList.push_back(relationship);
}

vector<Relationship *> Query::GetRelationships() { return relationshipList; }
