#pragma once

enum class FactorType { VARIABLE, CONSTANT, EXPRESSION };

class Factor {
private:
    const FactorType factorType;
    int value;
    bool hasComputedValue; // Used for lazy initialization of value

protected:
    // constructor
    Factor(FactorType factorType, int value);
    Factor(FactorType factorType);

public:
    // getters
    const FactorType getFactorType();
    const int getValue();
};
