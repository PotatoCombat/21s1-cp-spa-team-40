#include "common/model/Factor.h"
#include "common/model/relation/Relation.h"

class EqualsRelation : public Relation {

public:
    EqualsRelation(Factor *leftFactor, Factor *rightFactor);
    bool getValue() override;
};