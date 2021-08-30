#include "Variable.h"

Variable::Variable(int value, string name) : name(name), Factor(FactorType::VARIABLE, value) {}

string Variable::getName() {
    return this->name;
}