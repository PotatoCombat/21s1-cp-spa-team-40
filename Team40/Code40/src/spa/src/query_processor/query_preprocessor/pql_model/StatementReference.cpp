#include "StatementReference.h"

string StatementReference::getValue() { return value; }

ReferenceType StatementReference::getType() { return ReferenceType::STMT_REF; }
