#pragma once

#include "Entity.h"

enum class RelationshipType { UNKNOWN, FOLLOWS };

class Relationship {
private:
    Entity firstEntity;
    Entity secondEntity;

public:
    Relationship(Entity firstEntity, Entity secondEntity);
    Entity GetFirstEntity();
    Entity GetSecondEntity();
    virtual RelationshipType GetRelationshipType();
};
