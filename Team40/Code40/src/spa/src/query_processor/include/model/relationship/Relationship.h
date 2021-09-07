#pragma once

#include "Entity.h"

using namespace std;

enum class RelationshipType { FOLLOWS };

class Relationship {
protected:
    Entity* firstEntity;
    Entity* secondEntity;

public:
    Relationship(Entity* firstEntity, Entity* secondEntity);
    Entity* getFirstEntity();
    Entity* getSecondEntity();
    virtual RelationshipType getRelationshipType() = 0;
};
