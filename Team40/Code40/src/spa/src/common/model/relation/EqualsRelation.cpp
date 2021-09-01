#include "../Factor.h"
#include "Relation.h"

class EqualsRelation : public Relation {
public:
    EqualsRelation(Factor leftFactor, Factor rightFactor)
        : Relation(RelationType::EQUALS, leftFactor, rightFactor){};
    bool getValue() {
        return this->getLeftFactor().getValue() ==
               this->getRightFactor().getValue();
    }
};