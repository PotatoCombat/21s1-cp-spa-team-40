#pragma once

#include <string>
#include <vector>

#include "Entity.h"
#include "Relationship.h"

using namespace std;

// typedef vector<Entity> LIST_OF_ENTITY;

class Query {
private:
    vector<Entity *> entityList;
    vector<Relationship *> relationshipList;
    Entity *returnEntity;

public:
    Query();

    void SetReturnEntity(Entity *entity);
    Entity *GetReturnEntity();

    void AddEntity(Entity *entity);
    vector<Entity *> GetEntities();

    void AddRelationship(Relationship *relationship);
    vector<Relationship *> GetRelationships();
}
