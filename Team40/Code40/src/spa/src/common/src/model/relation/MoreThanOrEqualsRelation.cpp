#include "Relation.h"

class MoreThanOrEqualsRelation : public Relation {
public:
    MoreThanOrEqualsRelation(Factor leftFactor, Factor rightFactor)
        : Relation(RelationType::MORE_THAN_OR_EQUALS, leftFactor,
                   rightFactor){};

    bool getValue() {
        return this->getLeftFactor().getValue() >=
               this->getRightFactor().getValue();
    }
};
