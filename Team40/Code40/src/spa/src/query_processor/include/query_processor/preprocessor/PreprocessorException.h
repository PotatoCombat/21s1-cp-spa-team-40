#pragma once

#include <stdexcept>

using namespace std;

class PreprocessorException : public invalid_argument {
public:
    PreprocessorException(const string &msg) : invalid_argument(msg) {}
};