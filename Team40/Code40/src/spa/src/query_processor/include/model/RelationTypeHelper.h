#pragma once

#include <string>
#include <map>

#include "RelationType.h"
#include "DesignEntityType.h"

using namespace std;

class RelationTypeHelper {
private:
    map<RelationType, string> typeToStringMap;
    map<string, RelationType> stringToTypeMap;
    map<RelationType, DesignEntityType> deTypeMap1;
    map<RelationType, DesignEntityType> deTypeMap2;

public:
    RelationTypeHelper();

    // get type from string
    RelationType getType(string val);

    // get value from type
    string getValue(RelationType type);

    // get supposed deType from type for ref1 and ref2
    DesignEntityType chooseDeType1(RelationType type);
    DesignEntityType chooseDeType2(RelationType type);
};
