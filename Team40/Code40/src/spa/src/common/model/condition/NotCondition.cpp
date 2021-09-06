#include "NotCondition.h"

NotConditional::NotConditional(Conditional *cond)
    : cond(cond), Conditional(ConditionalType::NOT, !cond->getValue()) {}
