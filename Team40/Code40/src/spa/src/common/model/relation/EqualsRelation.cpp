#include "Relation.h"
#include "../expression/Expression.h"

class EqualsRelation : public Relation {
private:
    Expression expression1;
    Expression expression1;

public:
    EqualsRelation(Expression e1, Expression e2): expression1(e1), expression2(e2);
};