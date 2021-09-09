#include "common/model/relation/EqualsRelation.h"

EqualsRelation::EqualsRelation(Factor *leftFactor, Factor *rightFactor)
    : Relation(RelationType::EQUALS, leftFactor, rightFactor){};

bool EqualsRelation::getValue() {
    return getLeftFactor()->getValue() == getRightFactor()->getValue();
}
