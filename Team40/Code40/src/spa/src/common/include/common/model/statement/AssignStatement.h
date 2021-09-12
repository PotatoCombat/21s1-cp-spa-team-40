#include "common/model/Variable.h"
#include "common/model/expression/Expression.h"
#include "common/model/statement/Statement.h"
#include <string>
#include <vector>
using namespace std;

class AssignStatement : public Statement {
private:
    Variable *variable;
    vector<string> exprLst;

public:
    AssignStatement(int index, Variable *variable);

    void setExprLst(vector<string> exprLst);

    Variable *getVariable() override;

    vector<string> getExprList();
};
