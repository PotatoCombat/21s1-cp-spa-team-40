#include "common/model/relation/Relation.h"

class MoreThanOrEqualsRelation : public Relation {
public:
    MoreThanOrEqualsRelation(Factor *leftFactor, Factor *rightFactor)
        : Relation(RelationType::MORE_THAN_OR_EQUALS, leftFactor,
                   rightFactor){};
};
