#pragma once

#include <string>

using namespace std;

enum class DesignEntityType {
    UNKNOWN,
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

class Declaration {
protected:
    string syn;

public:
    Declaration(string syn);
    string getSynonym();
    virtual DesignEntityType getType();
};
