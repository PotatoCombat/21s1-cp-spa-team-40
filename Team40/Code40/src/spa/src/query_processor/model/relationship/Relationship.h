#pragma once

#include "../entity/Entity.h"

using namespace std;

enum class RelationshipType { FOLLOWS };

class Relationship {
private:
    Entity firstEntity;
    Entity secondEntity;

public:
    Relationship(Entity firstEntity, Entity secondEntity);
    Entity GetFirstEntity();
    Entity GetSecondEntity();
    virtual RelationshipType GetRelationshipType() = 0;
};
