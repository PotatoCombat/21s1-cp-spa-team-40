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

RelationTypeHelper::RelationTypeHelper() {
    typeToStringMap = {
        {RelationType::FOLLOWS, "Follows"},
        {RelationType::FOLLOWS_T, "Follows*"},
        {RelationType::PARENT, "Parent"},
        {RelationType::PARENT_T, "Parent*"},
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
    deTypeMap1 = {
        {RelationType::FOLLOWS, DesignEntityType::STMT},
        {RelationType::FOLLOWS_T, DesignEntityType::STMT},
        {RelationType::PARENT, DesignEntityType::STMT},
        {RelationType::PARENT_T, DesignEntityType::STMT},
        {RelationType::MODIFIES_P, DesignEntityType::PROCEDURE},
        {RelationType::MODIFIES_S, DesignEntityType::STMT},
        {RelationType::USES_P, DesignEntityType::PROCEDURE},
        {RelationType::USES_S, DesignEntityType::STMT}
    };
    deTypeMap2 = {
        {RelationType::FOLLOWS, DesignEntityType::STMT},
        {RelationType::FOLLOWS_T, DesignEntityType::STMT},
        {RelationType::PARENT, DesignEntityType::STMT},
        {RelationType::PARENT_T, DesignEntityType::STMT},
        {RelationType::MODIFIES_P, DesignEntityType::VARIABLE},
        {RelationType::MODIFIES_S, DesignEntityType::VARIABLE},
        {RelationType::USES_P, DesignEntityType::VARIABLE},
        {RelationType::USES_S, DesignEntityType::VARIABLE}
    };
}

RelationType RelationTypeHelper::getType(string val) {
    auto type = stringToTypeMap.find(val);
    if (type == stringToTypeMap.end()) {
        throw "invalid relation type";
    }
    return type->second;
}

string RelationTypeHelper::getValue(RelationType type) {
    auto val = typeToStringMap.find(type);
    if (val == typeToStringMap.end()) { // typically should not reach here
        throw "invalid relation type";
    }
    return val->second;
}

DesignEntityType RelationTypeHelper::chooseDeType1(RelationType type) {
    auto de = deTypeMap1.find(type);
    if (de == deTypeMap1.end()) {
        throw "invalid relation type";
    }
    return de->second;
}

DesignEntityType RelationTypeHelper::chooseDeType2(RelationType type) {
    auto de = deTypeMap2.find(type);
    if (de == deTypeMap2.end()) {
        throw "invalid relation type";
    }
    return de->second;
}
