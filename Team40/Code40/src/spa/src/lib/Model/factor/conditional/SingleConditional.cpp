#include "SingleConditional.h"

SingleConditional::SingleConditional(Conditional *cond)
    : cond(cond), Conditional(ConditionalType::SINGLE, cond->getValue()) {}
