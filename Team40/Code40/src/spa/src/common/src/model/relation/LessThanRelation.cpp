#include "common/model/relation/Relation.h"

class LessThanRelation : public Relation {
public:
    LessThanRelation(Factor leftFactor, Factor rightFactor)
        : Relation(RelationType::LESS_THAN, leftFactor, rightFactor){};

    bool getValue() {
        return this->getLeftFactor().getValue() <
               this->getRightFactor().getValue();
    }
};
