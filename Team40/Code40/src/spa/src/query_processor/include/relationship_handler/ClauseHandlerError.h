#pragma once

#include <stdexcept>

using namespace std;

class ClauseHandlerError : domain_error {
public:
    ClauseHandlerError(const std::string &what) : std::domain_error(what) {}
};
