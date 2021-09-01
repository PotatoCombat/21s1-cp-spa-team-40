#pragma once

enum class ConditionalType { SINGLE, AND, OR, NOT };

class Conditional {
private:
    ConditionalType condType;
    const bool value;

protected:
    // constructor
    Conditional(ConditionalType condType, bool value);

public:
    // getters
    ConditionalType getConditionalType();

    bool getValue();
};
