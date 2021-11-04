#pragma once

#include <string>
#include <unordered_map>

#include "query_processor/exception/SyntaxError.h"
#include "query_processor/exception/ValidityError.h"

#include "query_processor/model/ClauseType.h"
#include "query_processor/model/DesignEntityType.h"

using namespace std;

class ClauseTypeHelper {
private:
    unordered_map<string, ClauseType> stringToTypeMap;

    // only used to represent a wildcard/constant used in Clause
    // since it cannot be declared
    unordered_map<ClauseType, DesignEntityType> deTypeMap1;
    unordered_map<ClauseType, DesignEntityType> deTypeMap2;

public:
    ClauseTypeHelper();

    // get type from string
    ClauseType valueToClsType(string val);

    // get supposed deType from type for ref1 and ref2
    DesignEntityType chooseDeType1(ClauseType type);
    DesignEntityType chooseDeType2(ClauseType type);
};
