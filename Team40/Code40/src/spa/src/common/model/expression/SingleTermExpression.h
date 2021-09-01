#include "Expression.h"
#include "common/model/Term.h"

class SingleTermExpression : public Expression {
private:
    Term *term;

public:
    SingleTermExpression(Term *term);
};
