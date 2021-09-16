#include "query_processor/model/ClauseTypeHelper.h"

ClauseTypeHelper::ClauseTypeHelper() {
    typeToStringMap = {
        {ClauseType::FOLLOWS, "Follows"},
        {ClauseType::FOLLOWS_T, "Follows*"},
        {ClauseType::PARENT, "Parent"},
        {ClauseType::PARENT_T, "Parent*"},
        {ClauseType::MODIFIES_P, "Modifies"},
        {ClauseType::MODIFIES_S, "Modifies"},
        {ClauseType::USES_P, "Uses"},
        {ClauseType::USES_S, "Uses"}
    };
    stringToTypeMap = {
        {"Follows", ClauseType::FOLLOWS},
        {"Follows*", ClauseType::FOLLOWS_T},
        {"Parent", ClauseType::PARENT},
        {"Parent*", ClauseType::PARENT_T},
        {"Modifies", ClauseType::MODIFIES_S},
        {"Modifies*", ClauseType::MODIFIES_P}, // workaround differentiating S and P
        {"Uses", ClauseType::USES_S},
        {"Uses*", ClauseType::USES_P} // workaround differentiating S and P
    };
    deTypeMap1 = {
        {ClauseType::FOLLOWS, DesignEntityType::STMT},
        {ClauseType::FOLLOWS_T, DesignEntityType::STMT},
        {ClauseType::PARENT, DesignEntityType::STMT},
        {ClauseType::PARENT_T, DesignEntityType::STMT},
        {ClauseType::MODIFIES_P, DesignEntityType::PROCEDURE},
        {ClauseType::MODIFIES_S, DesignEntityType::STMT},
        {ClauseType::USES_P, DesignEntityType::PROCEDURE},
        {ClauseType::USES_S, DesignEntityType::STMT}
    };
    deTypeMap2 = {
        {ClauseType::FOLLOWS, DesignEntityType::STMT},
        {ClauseType::FOLLOWS_T, DesignEntityType::STMT},
        {ClauseType::PARENT, DesignEntityType::STMT},
        {ClauseType::PARENT_T, DesignEntityType::STMT},
        {ClauseType::MODIFIES_P, DesignEntityType::VARIABLE},
        {ClauseType::MODIFIES_S, DesignEntityType::VARIABLE},
        {ClauseType::USES_P, DesignEntityType::VARIABLE},
        {ClauseType::USES_S, DesignEntityType::VARIABLE}
    };
}

ClauseType ClauseTypeHelper::getType(string val) {
    auto type = stringToTypeMap.find(val);
    if (type == stringToTypeMap.end()) {
        throw ValidityError("invalid clause type");
    }
    return type->second;
}

string ClauseTypeHelper::getValue(ClauseType type) {
    auto val = typeToStringMap.find(type);
    if (val == typeToStringMap.end()) { // typically should not reach here
        throw "invalid clause type";
    }
    return val->second;
}

// only used with wildcard/constant/quoted arguments
DesignEntityType ClauseTypeHelper::chooseDeType1(ClauseType type) {
    auto de = deTypeMap1.find(type);
    if (de == deTypeMap1.end()) {
        throw "invalid clause type";
    }
    return de->second;
}

// only used with wildcard/constant/quoted arguments
DesignEntityType ClauseTypeHelper::chooseDeType2(ClauseType type) {
    auto de = deTypeMap2.find(type);
    if (de == deTypeMap2.end()) {
        throw "invalid clause type";
    }
    return de->second;
}
