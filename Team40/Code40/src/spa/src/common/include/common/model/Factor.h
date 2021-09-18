#pragma once
#include <string>

using namespace std;

enum class FactorType { VARIABLE, CONSTANT, EXPRESSION };

class Factor {
public:
    FactorType getFactorType();

protected:
    explicit Factor(FactorType factorType);

private:
    FactorType factorType;
};
