#pragma once

enum class FactorType { VARIABLE, CONSTANT, EXPRESSION };

class Factor {
private:
    const FactorType factorType;
    int value;
    bool knownValue;

protected:
    // constructor
    Factor(FactorType factorType, int value, bool knownValue);

public:
    // getters
    const FactorType getFactorType();
    const int getValue();
};
