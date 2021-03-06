#include "query_processor/model/Reference.h"

const string Reference::WILDCARD = "_";

Reference::Reference(DesignEntityType deType, ReferenceType refType,
                     string value, ReferenceAttribute attr)
    : value(value), deType(deType), refType(refType), attr(attr) {}

Reference::Reference(DesignEntityType deType, ReferenceType refType,
                     string value)
    : Reference(deType, refType, value, ReferenceAttribute::DEFAULT) {}

string Reference::getValue() { return this->value; }

DesignEntityType Reference::getDeType() { return this->deType; }

ReferenceType Reference::getRefType() { return this->refType; }

ReferenceAttribute Reference::getAttr() { return this->attr; }

bool Reference::equals(Reference &other) {
    // NOTE: not comparing referenceAttr
    return this->getValue() == other.getValue() &&
           this->getDeType() == other.getDeType() &&
           this->getRefType() == other.getRefType();
}

Reference *Reference::copy() {
    return new Reference(this->deType, this->refType, this->value, this->attr);
}

Reference::~Reference() {}
