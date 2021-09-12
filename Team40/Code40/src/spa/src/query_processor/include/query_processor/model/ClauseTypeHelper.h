#pragma once

#include <map>
#include <string>

#include "ClauseType.h"
#include "DesignEntityType.h"

using namespace std;

class ClauseTypeHelper {
private:
    map<ClauseType, string> typeToStringMap;
    map<string, ClauseType> stringToTypeMap;
    map<ClauseType, DesignEntityType> deTypeMap1;
    map<ClauseType, DesignEntityType> deTypeMap2;

public:
    ClauseTypeHelper();

    // get type from string
    ClauseType getType(string val);

    // get value from type
    string getValue(ClauseType type);

    // get supposed deType from type for ref1 and ref2
    DesignEntityType chooseDeType1(ClauseType type);
    DesignEntityType chooseDeType2(ClauseType type);
};
