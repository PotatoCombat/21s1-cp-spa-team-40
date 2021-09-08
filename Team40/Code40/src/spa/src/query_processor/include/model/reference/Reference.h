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

class Reference {
private:
    string value;
    DesignEntityType type;

public:
    static const string WILDCARD;

    Reference(string value, DesignEntityType type);
    string getValue();
    DesignEntityType getType();
    virtual bool isSynonym() = 0;
    bool equals(Reference &other);
};
