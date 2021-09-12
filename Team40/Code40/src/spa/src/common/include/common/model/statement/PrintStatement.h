#include "common/model/Variable.h"
#include "common/model/statement/Statement.h"
using namespace std;

class PrintStatement : public Statement {
private:
    Variable *variable;

public:
    PrintStatement(int index, Variable *variable);

    Variable *getVariable() override;
};
