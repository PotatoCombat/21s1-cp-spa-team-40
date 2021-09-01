#include "../expression/Expression.h"
#include "Relation.h"

class LessThanOrEqualsRelation : public Relation {
public:
    LessThanOrEqualsRelation(Factor leftFactor, Factor rightFactor)
        : Relation(RelationType::LESS_THAN_OR_EQUALS, leftFactor,
                   rightFactor){};

    bool getValue() {
        return this->getLeftFactor() <= this->getRightFactor().getValue();
    }
};
