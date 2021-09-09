#pragma once
#include "Factor.h"
#include <string>
using namespace std;

class Variable : public Factor {
private:
    const string name;

public:
    Variable(int value, string name);
    Variable(string name);

    // getter
    string getName() override;
};
