#include "common/model/statement/ReadStatement.h"

ReadStatement::ReadStatement(int index, Variable *variable)
    : variable(variable), Statement(index, StatementType::READ) {}

Variable *ReadStatement::getVariable() { return variable; }