#include "AndCondition.h"

AndConditional::AndConditional(Conditional *cond1, Conditional *cond2)
    : cond1(cond1), cond2(cond2),
      Conditional(ConditionalType::AND,
                  cond1->getValue() && cond2->getValue()) {}
