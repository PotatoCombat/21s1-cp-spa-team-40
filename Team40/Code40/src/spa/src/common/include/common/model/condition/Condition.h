#pragma once

#include <stdexcept>

#include "common/model/relation/Relation.h"
#include <stdexcept>

enum class ConditionType { SINGLE, AND, OR, NOT };

class Condition {
private:
    ConditionType condType;

protected:
    // constructor
    Condition(ConditionType condType);

public:
    // getters
    ConditionType getConditionType();

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
