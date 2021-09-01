#pragma once

enum class ConditionalType { SINGLE, AND, OR, NOT };

class Conditional {
private:
    ConditionalType condType;
    const bool value;

public:
    // constructor
    Conditional(ConditionalType condType, bool value);

    // getters
    ConditionalType getConditionalType();

    bool getValue();
};
