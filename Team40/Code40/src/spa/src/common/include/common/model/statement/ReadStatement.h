#include "common/model/Variable.h"
#include "common/model/statement/Statement.h"
using namespace std;

class ReadStatement : public Statement {
private:
    Variable *variable;

public:
    ReadStatement(int index, Variable *variable);

    Variable *getVariable() override;
};
