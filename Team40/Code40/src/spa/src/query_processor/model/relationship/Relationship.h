#pragma once

#include "../entity/Entity.h"

using namespace std;

enum class RelationshipType { FOLLOWS };

class Relationship {
private:
    Entity* firstEntity;
    Entity* secondEntity;

public:
    Relationship(Entity* firstEntity, Entity* secondEntity);
    Entity* getFirstEntity();
    Entity* getSecondEntity();
    virtual RelationshipType getRelationshipType() = 0;
};
