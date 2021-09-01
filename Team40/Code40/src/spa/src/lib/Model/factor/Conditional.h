#pragma once

enum class ConditionalType { SINGLE, AND, OR, NOT };

class Conditional {
private:
    ConditionalType condType;
    // Relational rel;      // for SINGLE only
    // Conditional cond1; // for NOT, AND and OR
    // Conditional cond2; // for AND and OR only

public:
    // constructor
    Conditional(ConditionalType condType);

    // getters
    ConditionalType getConditionalType();
};
