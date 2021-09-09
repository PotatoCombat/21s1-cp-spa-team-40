#pragma once

#include <string>

using namespace std;

enum class DesignEntityType {
    STMT,
    ASSIGN,
    VARIABLE,
    CONSTANT,
    PROCEDURE,
    READ,
    PRINT,
    WHILE,
    IF,
    CALL
};

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
};
