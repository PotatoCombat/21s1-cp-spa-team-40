#pragma once

#include <map>
#include <string>

#include "query_processor/exception/ValidityError.h"
#include "ClauseType.h"
#include "DesignEntityType.h"
#include "query_processor/parser/RefType.h"

using namespace std;

class ClauseTypeHelper {
private:
    map<ClauseType, string> typeToStringMap;
    map<string, ClauseType> stringToTypeMap;

    // only used to represent a wildcard/constant used in Clause 
    // since it cannot be declared
    map<ClauseType, DesignEntityType> deTypeMap1;
    map<ClauseType, DesignEntityType> deTypeMap2;

public:
    ClauseTypeHelper();

    // get type from string
    ClauseType getType(string val);

    // get type from clause string, first ref type, first de type
    ClauseType getType(string val, RefType rt, DesignEntityType det);

    // get value from type
    string getValue(ClauseType type);

    // get supposed deType from type for ref1 and ref2
    DesignEntityType chooseDeType1(ClauseType type);
    DesignEntityType chooseDeType2(ClauseType type);
};
