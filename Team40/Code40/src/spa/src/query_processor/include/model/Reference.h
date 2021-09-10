#pragma once

#include <string>

#include "DesignEntityType.h"

using namespace std;

enum class ReferenceType {
    SYNONYM,
    CONSTANT, 
    WILDCARD
};

class Reference {
private:
    DesignEntityType deType;
    ReferenceType refType;
    string value;
    
public:
    static const string WILDCARD;
    Reference(DesignEntityType deType, ReferenceType refType, string value);
    string getValue();
    DesignEntityType getDeType();
    ReferenceType getRefType();
    bool equals(Reference &other);
    ~Reference();
};
