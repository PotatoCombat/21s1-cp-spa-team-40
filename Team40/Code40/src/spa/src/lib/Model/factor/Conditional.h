#pragma once

enum class ConditionalType { SINGLE, AND, OR, NOT };

class Conditional {
private:
    ConditionalType condType;
    // Relational relExpr;      // for SINGLE only
    // Conditional condExpr1; // for NOT, AND and OR
    // Conditional condExpr2; // for AND and OR only

public:
    // constructor
    Conditional(ConditionalType condType);

    // getters
    ConditionalType getCondType();
};
