#include "SingleConditional.h"

SingleConditional::SingleConditional(Relation *rel)
    : rel(rel), Conditional(ConditionalType::SINGLE, rel->getValue()) {}
