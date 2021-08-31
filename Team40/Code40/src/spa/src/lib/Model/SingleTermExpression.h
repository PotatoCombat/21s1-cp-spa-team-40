#include "Expression.h"
#include "Term.h"

class SingleTermExpression : public Expression {
    private:
    Term *term;

    public:
    SingleTermExpression(Term *term);
};
