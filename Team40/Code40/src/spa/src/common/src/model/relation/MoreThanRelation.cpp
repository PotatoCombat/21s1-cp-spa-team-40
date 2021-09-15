#include "common/model/relation/Relation.h"

class MoreThanRelation : public Relation {
public:
    MoreThanRelation(Factor *leftFactor, Factor *rightFactor)
        : Relation(RelationType::MORE_THAN_OR_EQUALS, leftFactor,
                   rightFactor){};
};
