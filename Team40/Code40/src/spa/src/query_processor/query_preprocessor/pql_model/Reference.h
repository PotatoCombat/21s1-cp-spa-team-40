#pragma once

#include <string>

using namespace std;

enum class ReferenceType { UNKNOWN, ENT_REF, STMT_REF };

class Reference {
protected:
    string value;

public:
    Reference(string value);
    string getValue();
    virtual ReferenceType getType();
};
