#include "Expression.h"
#include "common/model/term/Term.h"

class SingleTermExpression : public Expression {
private:
    Term *term;

public:
    SingleTermExpression(Term *term);

    Term *getTerm();
};
