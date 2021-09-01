#include "Relation.h"

class LessThan : public Relation {
public:
    LessThan(Factor leftFactor, Factor rightFactor)
        : Relation(RelationType::LESS_THAN, leftFactor, rightFactor){};

    bool getValue() {
        return this->getLeftFactor().getValue() <
               this->getRightFactor().getValue();
    }
};
