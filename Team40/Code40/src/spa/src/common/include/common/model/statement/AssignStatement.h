#include "common/model/Variable.h"
#include "common/model/expression/Expression.h"
#include "common/model/statement/Statement.h"
#include <string>
#include <vector>
using namespace std;

class AssignStatement : public Statement {
private:
    Variable *variable;
    Expression *expression;
    vector<string> exprLst;

public:
    AssignStatement(int index, Variable *variable, Expression *expression);

    AssignStatement(int index, Variable *variable);

    void setExprLst(vector<string> exprLst);

    Variable *getVariable() override;

    Expression *getExpression() override;
};
