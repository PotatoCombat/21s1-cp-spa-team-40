#include "Reference.h"

const string Reference::WILDCARD = "_";

Reference::Reference(string value, DesignEntityType type)
    : value(value), type(type) {}

string Reference::getValue() { return this->value; }

DesignEntityType Reference::getType() { return this->type; }

bool Reference::equals(Reference &other) {
    return this->getValue() == other.getValue() &&
           this->getType() == other.getType() &&
           this->isSynonym() == other.isSynonym();
}
