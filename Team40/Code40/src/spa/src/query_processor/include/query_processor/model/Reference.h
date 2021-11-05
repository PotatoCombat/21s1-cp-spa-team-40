#pragma once

#include <string>

#include "query_processor/model/DesignEntityType.h"

using namespace std;

enum class ReferenceType { SYNONYM, CONSTANT, WILDCARD };

enum class ReferenceAttribute { NAME, INTEGER, DEFAULT };

/**
 * Represents a reference object that is used in the clauses
 */
class Reference {
private:
    DesignEntityType deType;
    ReferenceType refType;
    string value;
    ReferenceAttribute attr;

public:
    static const string WILDCARD;
    Reference(DesignEntityType deType, ReferenceType refType, string value,
              ReferenceAttribute attr);
    Reference(DesignEntityType deType, ReferenceType refType, string value);
    string getValue();
    DesignEntityType getDeType();
    ReferenceType getRefType();
    ReferenceAttribute getAttr();
    // NOTE: not comparing referenceAttr
    bool equals(Reference &other);
    Reference *copy();
    ~Reference();
};
