#include "common/model/relation/Relation.h"

Relation::Relation(RelationType relType, Factor leftFactor, Factor rightFactor)
    : relType(relType), leftFactor(leftFactor), rightFactor(rightFactor) {}

RelationType Relation::getRelationType() { return this->relType; }

Factor Relation::getLeftFactor() { return this->leftFactor; }

Factor Relation::getRightFactor() { return this->rightFactor; }