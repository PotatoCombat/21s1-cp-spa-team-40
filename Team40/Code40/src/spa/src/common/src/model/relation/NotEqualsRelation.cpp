#include "model/relation/Relation.h"

class NotEqualsRelation : public Relation {
public:
    NotEqualsRelation(Factor leftFactor, Factor rightFactor)
        : Relation(RelationType::MORE_THAN_OR_EQUALS, leftFactor,
                   rightFactor){};

    bool getValue() {
        return this->getLeftFactor().getValue() !=
               this->getRightFactor().getValue();
    }
};
