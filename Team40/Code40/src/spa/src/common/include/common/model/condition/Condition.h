#pragma once

#include <stdexcept>

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

    // Overriden by: SingleCondition
    virtual Relation *getRelation() {
        throw runtime_error(
            "This method is not implemented for this ConditionType.");
    };

    // Overriden by: NotCondition, AndCondition, OrCondition
    virtual Condition *getPrimaryCondition() {
        throw runtime_error(
            "This method is not implemented for this ConditionType");
    }

    // Overriden by: AndCondition, OrCondition
    virtual Condition *getSecondaryCondition() {
        throw runtime_error(
            "This method is not implemented for this ConditionType");
    }
};
