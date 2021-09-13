#pragma once

#include <stdexcept>
#include <string>

using namespace std;

enum class FactorType { VARIABLE, CONSTANT, EXPRESSION };

class Factor {
private:
    const FactorType factorType;
    int value;
    bool hasComputedValue; // Used for lazy initialization of value

protected:
    // constructor
    explicit Factor(FactorType factorType);

public:
    // getters
    FactorType getFactorType();
    int getValue() const;
    virtual string getName() {
        throw runtime_error(
            "This method is not available for this FactorType.");
    }
};
