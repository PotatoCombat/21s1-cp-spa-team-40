#include "Variable.h"

Variable::Variable(int value, int name) : value(value), name(name), Factor(FactorType::VARIABLE) {}

int Variable::getValue() {
   return this->value;
}

string Variable::getName() {
    return this->name;
}