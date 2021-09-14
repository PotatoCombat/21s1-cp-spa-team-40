#include "common/model/Variable.h"
using namespace std;

Variable::Variable(int value, string name)
    : name(name), Factor(FactorType::VARIABLE) {}

Variable::Variable(string name) : name(name), Factor(FactorType::VARIABLE) {}

string Variable::getName() { return this->name; }

bool Variable::operator< (const Variable& other) const {
    return name < other.name;
}

bool Variable::operator== (const Variable& other) const {
    return name == other.name;
}
