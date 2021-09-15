#pragma once

#include "query_processor/Abstractions.h"

#include "query_processor/exception/ValidityError.h"
#include "query_processor/model/Clause.h"
#include "query_processor/model/ClauseTypeHelper.h"
#include "query_processor/model/DesignEntityTypeHelper.h"
#include "query_processor/model/Reference.h"

class SuchThatParser {
protected:
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;
    ReferenceType checkRefType(string val);
    bool isInteger(string val);
    bool isQuoted(string val);
    bool isWildcard(string val);
    vector<Reference *> declList;

public:
    SuchThatParser() = default();
    SuchThatParser(DesignEntityTypeHelper *deHelper, ClauseTypeHelper *clsHelper);
    virtual Clause parse();
};
