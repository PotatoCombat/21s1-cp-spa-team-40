#pragma once

#include "common/model/Variable.h"
#include "common/model/condition/Condition.h"
#include <stdexcept>
#include <vector>

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
        throw runtime_error("getProcName() is not implemented.");
    }

    // Overriden by: PrintStatement, ReadStatement, AssignStatement
    virtual Variable getVariable() {
        throw runtime_error("getVariable() is not implemented.");
    };

    // Overriden by: IfStatement, WhileStatement
    virtual Condition getCondExpr() {
        throw runtime_error("getCondExpr() is not implemented.");
    }

    // Overriden by: IfStatement
    virtual vector<Statement> getThenStmtLst() {
        throw runtime_error("getThenStmtLst() is not implemented.");
    }

    // Overriden by: IfStatement
    virtual vector<Statement> getElseStmtLst() {
        throw runtime_error("getElseStmtLst() is not implemented.");
    }

    // Overriden by: WhileSatement
    virtual vector<Statement> getStmtLst() {
        throw runtime_error("getStmtLst() is not implemented.");
    }
};
