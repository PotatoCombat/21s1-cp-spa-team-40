#pragma once
#include <string>
#include "Factor.h"
using namespace std;

class ConstantValue : public Factor {
public:
    explicit ConstantValue(string name);

    string getName();

    bool operator<(const ConstantValue &other) const;
    bool operator==(const ConstantValue &other) const;

private:
    string name;
};
