#include "common/model/ConstantValue.h"
#include "common/model/Variable.h"
#include "common/model/statement/Statement.h"
#include <string>
#include <vector>

using namespace std;

class AssignStatement : public Statement {
private:
    Variable *variable;
    vector<string> exprLst;
    vector<Variable> varLst;
    vector<ConstantValue> constLst;

public:
    AssignStatement(int index, Variable *variable);

    void setExprLst(vector<string> exprLst);

    void addVar(Variable var);

    void addConst(ConstantValue constVal);

    Variable *getVariable() override;
};
