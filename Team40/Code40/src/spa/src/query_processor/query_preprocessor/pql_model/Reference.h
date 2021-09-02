#pragma once

#include <string>

using namespace std;

class Reference {
private:
    string value;

public:
    Reference(string value);
    string getValue();
};
