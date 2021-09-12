#include "common/model/relation/Relation.h"

class NotEqualsRelation : public Relation {
public:
    NotEqualsRelation(Factor *leftFactor, Factor *rightFactor)
        : Relation(RelationType::MORE_THAN_OR_EQUALS, leftFactor,
                   rightFactor){};

    bool getValue() {
        return getLeftFactor()->getValue() != getRightFactor()->getValue();
    }
};
