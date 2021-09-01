#include "SingleConditional.h"

SingleConditional::SingleConditional(Relational *rel)
    : rel(rel), Conditional(ConditionalType::SINGLE, rel->getValue()) {}
