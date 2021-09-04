#include "Variable.h"

Variable::Variable(int value, string name)
    : name(name), Factor(FactorType::VARIABLE, value, true) {}

Variable::Variable(string name)
    : name(name), Factor(FactorType::VARIABLE, 0, false) {}

string Variable::getName() { return this->name; }
