#pragma once
#include "common/model/relation/Relation.h"

enum class ConditionType { SINGLE, AND, OR, NOT };

class Condition {
private:
    ConditionType condType;
    const bool value;

protected:
    // constructor
    Condition(ConditionType condType, bool value);

public:
    // getters
    ConditionType getConditionType();

    bool getValue();

    virtual Relation *getRelation() {
        throw runtime_error(
            "This method is not implemented for this ConditionType.");
    };
};
