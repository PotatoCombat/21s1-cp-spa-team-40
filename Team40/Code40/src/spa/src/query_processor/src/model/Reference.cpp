#include "Reference.h"

const string Reference::WILDCARD = "_";

Reference::Reference(DesignEntityType deType, ReferenceType refType, string value)
    : value(value), deType(deType), refType(refType) {}

string Reference::getValue() { return this->value; }

DesignEntityType Reference::getDeType() { return this->deType; }

ReferenceType Reference::getRefType() { return this->refType; }

bool Reference::equals(Reference &other) {
    return this->getValue() == other.getValue() &&
        this->getDeType() == other.getDeType() &&
        this->getRefType() == other.getRefType();
}

Reference::~Reference() {}
