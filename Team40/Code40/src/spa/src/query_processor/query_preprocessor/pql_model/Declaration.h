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
    DesignEntityType type;

public:
    Declaration(string syn, DesignEntityType type);
    string getSynonym();
    DesignEntityType getType();
};
