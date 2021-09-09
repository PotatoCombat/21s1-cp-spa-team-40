#pragma once

#include <string>
#include <map>

#include "DesignEntityType.h"

using namespace std;

class DesignEntityTypeHelper {
private:
    map<DesignEntityType, string> typeToStringMap;
    map<string, DesignEntityType> stringToTypeMap;

public:
    DesignEntityTypeHelper();

    // get type from string
    DesignEntityType getType(string val);

    // get value from type
    string getValue(DesignEntityType type);
};

DesignEntityTypeHelper::DesignEntityTypeHelper() {
    typeToStringMap = {
        {DesignEntityType::STMT, "stmt"},
        {DesignEntityType::ASSIGN, "assign"},
        {DesignEntityType::VARIABLE, "variable"},
        {DesignEntityType::CONSTANT, "constant"},
        {DesignEntityType::PROCEDURE, "procedure"},
        {DesignEntityType::READ, "read"},
        {DesignEntityType::PRINT, "print"},
        {DesignEntityType::WHILE, "while"},
        {DesignEntityType::IF, "if"},
        {DesignEntityType::CALL, "call"}
    };
    stringToTypeMap = {
        {"stmt", DesignEntityType::STMT},
        {"assign", DesignEntityType::ASSIGN},
        {"variable", DesignEntityType::VARIABLE},
        {"constant", DesignEntityType::CONSTANT},
        {"procedure", DesignEntityType::PROCEDURE},
        {"read", DesignEntityType::READ},
        {"print", DesignEntityType::PRINT},
        {"while", DesignEntityType::WHILE},
        {"if", DesignEntityType::IF},
        {"call", DesignEntityType::CALL}
    };
}

DesignEntityType DesignEntityTypeHelper::getType(string val) {
    auto type = stringToTypeMap.find(val);
    if (type == stringToTypeMap.end()) {
        throw "invalid design entity type";
    }
    return type->second;
}

string DesignEntityTypeHelper::getValue(DesignEntityType type) {
    auto val = typeToStringMap.find(type);
    if (val == typeToStringMap.end()) { // typically should not reach here
        throw "invalid design entity type";
    }
    return val->second;
}
