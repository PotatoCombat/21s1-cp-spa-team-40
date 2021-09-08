//#include "common/model/Variable.h"
#include "../../include/common/model/Variable.h"

Variable::Variable(int value, string name)
    : name(name), Factor(FactorType::VARIABLE, value) {}

Variable::Variable(string name) : name(name), Factor(FactorType::VARIABLE) {}

string Variable::getName() { return this->name; }
