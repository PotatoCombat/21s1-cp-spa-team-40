#pragma once

#include <string>
#include <map>

#include "RelationType.h"

using namespace std;

class RelationTypeHelper {
private:
    map<RelationType, string> typeToStringMap;
    map<string, RelationType> stringToTypeMap;

public:
    RelationTypeHelper();

    // get type from string
    RelationType getType(string val);

    // get value from type
    string getValue(RelationType type);
};

RelationTypeHelper::RelationTypeHelper() {
    typeToStringMap = {
        {RelationType::FOLLOWS, "Follows"},
        {RelationType::FOLLOWS_T, "Follows*"},
        {RelationType::PARENT, "Parent"},
        {RelationType::PARENT_T, "Parent"},
        {RelationType::MODIFIES_P, "Modifies"},
        {RelationType::MODIFIES_S, "Modifies"},
        {RelationType::USES_P, "Uses"},
        {RelationType::USES_S, "Uses"}
    };
    stringToTypeMap = {
        {"Follows", RelationType::FOLLOWS},
        {"Follows*", RelationType::FOLLOWS_T},
        {"Parent", RelationType::PARENT},
        {"Parent*", RelationType::PARENT_T},
        //{RelationType::MODIFIES_P, "Modifies"},
        {"Modifies", RelationType::MODIFIES_S},
        //{RelationType::USES_P, "Uses"},
        {"Uses", RelationType::USES_S}
    };
}

RelationType RelationTypeHelper::getType(string val) {
    auto type = stringToTypeMap.find(val);
    if (type == stringToTypeMap.end()) {
        throw "invalid design entity type";
    }
    return type->second;
}

string RelationTypeHelper::getValue(RelationType type) {
    auto val = typeToStringMap.find(type);
    if (val == typeToStringMap.end()) { // typically should not reach here
        throw "invalid design entity type";
    }
    return val->second;
}
