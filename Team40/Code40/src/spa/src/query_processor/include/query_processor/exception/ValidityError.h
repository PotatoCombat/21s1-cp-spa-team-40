#pragma once

#include <stdexcept>

using namespace std;

class ValidityError : public invalid_argument {
public:
    ValidityError(const string& msg) : invalid_argument(msg) {}
};
