#pragma once

enum class FactorType { VARIABLE, CONSTANT, EXPRESSION };

class Factor {
private:
  const FactorType factorType;
  const int value;

public:
  // constructor
  Factor(FactorType factorType, int value);

  // getters
  const FactorType getFactorType();
  const int getValue();
};
