#pragma once

#include <stdexcept>
#include <string>

using namespace std;

enum class FactorType { VARIABLE, CONSTANT, EXPRESSION };

class Factor {
private:
    FactorType factorType;

protected:
    // constructor
    explicit Factor(FactorType factorType);

public:
    // getters
    FactorType getFactorType();
    virtual string getName() {
        throw runtime_error(
            "This method is not available for this FactorType.");
    }
};
