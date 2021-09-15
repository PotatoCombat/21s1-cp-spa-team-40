#include "common/model/relation/Relation.h"

class LessThanRelation : public Relation {
public:
    LessThanRelation(Factor *leftFactor, Factor *rightFactor)
        : Relation(RelationType::LESS_THAN, leftFactor, rightFactor){};
};
