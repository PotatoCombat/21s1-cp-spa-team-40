#include "common/model/Variable.h"
#include <utility>
using namespace std;

Variable::Variable(string name) : Factor(FactorType::VARIABLE) {
    this->name = move(name);
}

string Variable::getName() { return this->name; }

bool Variable::operator<(const Variable &other) const {
    return name < other.name;
}

bool Variable::operator==(const Variable &other) const {
    return name == other.name;
}
