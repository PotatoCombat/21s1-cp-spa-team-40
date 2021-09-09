#include "common/model/statement/Statement.h"

// constructor
Statement::Statement(int index, StatementType statementType)
    : index(index), statementType(statementType) {}

Statement::Statement(StatementType statementType)
    : statementType(statementType) {
    this->index = -1;
}

Statement::Statement() {
    this->index = -1;
    this->statementType = StatementType::NONE;
}

// getters
int Statement::getIndex() { return this->index; }

StatementType Statement::getStatementType() { return this->statementType; }
