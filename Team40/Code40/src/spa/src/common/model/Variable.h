#pragma once
#include <string>

#include "Factor.h"

using namespace std;

class Variable : public Factor {
private:
    const string name;

public:
    Variable(int value, string name);

    // getter
    string getName();
};
