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
