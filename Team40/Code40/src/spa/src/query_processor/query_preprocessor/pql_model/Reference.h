#pragma once

#include <string>

using namespace std;

enum class ReferenceType { UNKNOWN, ENT_REF, STMT_REF };

class Reference {
private:
    string value;

public:
    Reference(string value);

    string getValue();
    // TODO: try to not assign a value in cpp and make it completely virtual...?
    virtual ReferenceType getType();
    
    bool operator==(const Reference& other) const;
};
