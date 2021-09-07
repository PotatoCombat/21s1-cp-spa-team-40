#include "Statement.h"
#include "Procedure.h"

class CallStatement : public Statement {
private:
    Procedure proc;

public:
    CallStatement(int index, Procedure proc)
        : proc(proc), Statement(index, StatementType::CALL) {}

    Procedure getProcedure() { return this->proc; }
};
