#include "Procedure.h"
#include "Statement.h"

class CallStatement : public Statement {
private:
    Procedure proc;

public:
    CallStatement::CallStatement(int index, Procedure proc)
        : proc(proc), Statement(index, StatementType::CALL) {}

    Procedure getProcedure() { return this->proc; }
};
