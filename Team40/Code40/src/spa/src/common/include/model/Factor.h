#pragma once

enum class FactorType { VARIABLE, CONSTANT, EXPRESSION };

class Factor {
private:
    const FactorType factorType;
    const int value;

protected:
    // constructor
    Factor(FactorType factorType, int value);

public:
    // getters
    const FactorType getFactorType();
    const int getValue();
};
