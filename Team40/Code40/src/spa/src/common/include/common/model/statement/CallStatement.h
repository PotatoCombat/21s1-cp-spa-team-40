#include "common/model/Procedure.h"
#include "common/model/statement/Statement.h"
using namespace std;

class CallStatement : public Statement {
private:
    Procedure proc;

public:
    CallStatement(int index, Procedure proc)
        : proc(proc), Statement(index, StatementType::CALL) {}

    Procedure getProcedure() { return this->proc; }
};
