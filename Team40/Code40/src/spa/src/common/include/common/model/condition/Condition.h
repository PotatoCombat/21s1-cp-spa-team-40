#pragma once

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
};
