#pragma once
#include <string>
#include "Factor.h"
using namespace std;

class Variable : public Factor {
public:
    explicit Variable(string name);

    string getName();

    bool operator<(const Variable &other) const;
    bool operator==(const Variable &other) const;

private:
    string name;
};
