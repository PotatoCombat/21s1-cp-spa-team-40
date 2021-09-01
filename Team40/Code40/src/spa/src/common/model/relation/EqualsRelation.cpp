#include "../expression/Expression.h"
#include "Relation.h"

class EqualsRelation : public Relation {
private:
    Expression expression1;
    Expression expression2;

public:
    EqualsRelation(Expression e1, Expression e2)
        : expression1(e1), expression2(e2),
          Relation(RelationType::EQUALS,
                   expression1.getValue() == expression2.getValue()){};
};