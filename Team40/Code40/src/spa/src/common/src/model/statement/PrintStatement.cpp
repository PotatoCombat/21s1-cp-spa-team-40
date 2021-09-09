#include "common/model/statement/PrintStatement.h"

PrintStatement::PrintStatement(int index, Variable *variable)
    : variable(variable), Statement(index, StatementType::PRINT) {}

Variable *PrintStatement::getVariable() { return this->variable; }