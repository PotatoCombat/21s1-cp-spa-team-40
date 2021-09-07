#include "Expression.h"
#include "model/term/Term.h"

class SingleTermExpression : public Expression {
private:
    Term *term;

public:
    SingleTermExpression(Term *term);
};
