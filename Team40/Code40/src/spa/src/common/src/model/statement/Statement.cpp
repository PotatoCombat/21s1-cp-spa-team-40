#include "common/model/statement/Statement.h"

// constructor
Statement::Statement(int index, StatementType statementType)
    : index(index), statementType(statementType) {}

// getters
int Statement::getIndex() { return this->index; }

StatementType Statement::getStatementType() { return this->statementType; }
