using namespace std;

#include "Relationship.h"
#include "Entity.h"

Relationship::Relationship(Entity firstEntity, Entity secondEntity) {
    this->firstEntity = firstEntity;
    this->secondEntity = secondEntity;
}

Entity Relationship::GetFirstEntity() { return firstEntity; }

Entity Relationship::GetSecondEntity() { return secondEntity; }

RelationshipType Relationship::GetRelationshipType() {
    return RelationshipType::UNKNOWN;
}
