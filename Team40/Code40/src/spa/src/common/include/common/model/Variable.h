#pragma once
#include "Factor.h"
#include <string>
using namespace std;

class Variable : public Factor {
private:
    string name;

public:
    Variable(string name);

    // getter
    string getName() override;

    bool operator<(const Variable &other) const;
    bool operator==(const Variable &other) const;
};
