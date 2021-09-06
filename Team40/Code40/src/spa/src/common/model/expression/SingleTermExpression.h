#include "../term/Term.h"
#include "Expression.h"


class SingleTermExpression : public Expression {
private:
    Term *term;

public:
    SingleTermExpression(Term *term);
};
