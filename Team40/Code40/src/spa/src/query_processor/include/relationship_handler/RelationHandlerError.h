#pragma once

#include <stdexcept>

using namespace std;

class RelationHandlerError : domain_error {
public:
    RelationHandlerError(const std::string &what) : std::domain_error(what) {}
};