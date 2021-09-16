#pragma once

#include <stdexcept>

using namespace std;

class SyntaxError : public invalid_argument {
public:
    SyntaxError(const string &msg) : invalid_argument(msg) {}
};
