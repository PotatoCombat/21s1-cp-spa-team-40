#pragma once

#include "Entity.h"
#include "Relationship.h"

using namespace std;

class FollowsRelationship : public Relationship {
public:
    FollowsRelationship(Entity *firstEntity, Entity *secondEntity)
        : Relationship(firstEntity, secondEntity) {}
    RelationshipType getRelationshipType() { return RelationshipType::FOLLOWS; }
};
