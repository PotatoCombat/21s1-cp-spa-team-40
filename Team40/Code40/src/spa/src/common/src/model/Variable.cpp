#include "common/model/Variable.h"

#include <utility>

using namespace std;

Variable::Variable(VarName name) : Entity<VarName>(move(name)) { }

bool Variable::operator<(const Variable &other) const {
    return name < other.name;
}

bool Variable::operator==(const Variable &other) const {
    return name == other.name;
}
