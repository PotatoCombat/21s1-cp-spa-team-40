#include "Reference.h"

Reference::Reference(string value) { this->value = value; }

string Reference::getValue() { return this->value; }

ReferenceType Reference::getType() { return ReferenceType::UNKNOWN; }

bool Reference::operator==(const Reference& other) const {
    return this->value == other.value;
}
