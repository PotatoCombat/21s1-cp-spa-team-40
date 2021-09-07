#include "Relationship.h"

Relationship::Relationship(Entity* firstEntity, Entity* secondEntity) {
    this->firstEntity = firstEntity;
    this->secondEntity = secondEntity;
}

Entity* Relationship::getFirstEntity() { return firstEntity; }

Entity* Relationship::getSecondEntity() { return secondEntity; }
