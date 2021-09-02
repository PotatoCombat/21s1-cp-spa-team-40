#pragma once

#include <string>

using namespace std;

enum class ReferenceType { ENT_REF, STMT_REF };

class Reference {
private:
    string value;

public:
    Reference(string value) { this->value = value; }
    string getValue() { return value; }
    virtual ReferenceType getType() = 0;
};
