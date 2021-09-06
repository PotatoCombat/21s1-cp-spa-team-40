#include "OrConditional.h"

OrConditional::OrConditional(Conditional *cond1, Conditional *cond2)
    : cond1(cond1), cond2(cond2),
      Conditional(ConditionalType::OR, cond1->getValue() || cond2->getValue()) {
}
