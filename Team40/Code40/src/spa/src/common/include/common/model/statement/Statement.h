#pragma once

#include "common/model/Variable.h"
#include "common/model/condition/Condition.h"
#include "common/model/expression/Expression.h"
#include <vector>
#include <stdexcept>

enum class StatementType { UNKNOWN, READ, PRINT, ASSIGN, CALL, WHILE, IF };

class Statement {
private:
    StatementType statementType;
    int index;

protected:
public:
    // constructor
    Statement(int index, StatementType statementType);

    // getters
    int getIndex();

    StatementType getStatementType();

    // Overriden by: CallStatement
    virtual string getProcName() { // return name instead of object to prevent
                                   // circular dependencies
        throw runtime_error(
            "This method is not implemented for this StatementType.");
    }

    // Overriden by: PrintStatement, ReadStatement, AssignStatement
    virtual Variable *getVariable() {
        throw runtime_error(
            "This method is not implemented for this StatementType.");
    };

    // Overriden by: IfStatement, WhileStatement
    virtual Condition *getCondition() {
        throw runtime_error(
            "This method is not implemented for this StatementType.");
    }

    virtual Expression *getExpression() {
        throw runtime_error(
            "This method is not implemented for this StatementType.");
    }

    // Overriden by: IfStatement, WhileStatement
    virtual vector<Statement> getThenStmtLst() {
        throw runtime_error(
            "This method is not implemented for this StatementType.");
    }

    // Overriden by: IfStatement
    virtual vector<Statement> getElseStmtLst() {
        throw runtime_error(
            "This method is not implemented for this StatementType.");
    }
};
