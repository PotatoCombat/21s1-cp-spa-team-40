#include "Relationship.h"

Relationship::Relationship(Entity* firstEntity, Entity* secondEntity) {
    this->firstEntity = firstEntity;
    this->secondEntity = secondEntity;
}

Entity* Relationship::GetFirstEntity() { return firstEntity; }

Entity* Relationship::GetSecondEntity() { return secondEntity; }
