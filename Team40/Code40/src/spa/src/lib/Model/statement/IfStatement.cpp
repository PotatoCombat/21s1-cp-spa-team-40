#include "Statement.h"
#include "factor/expression/ConditionalExpression.h"
#include <vector>

class IfStatement : public Statement {
private:
    ConditionalExpression cond; //TODO
    vector<Statement> thenStmtLst;
    vector<Statement> elseStmtLst;

public:
    IfStatement::IfStatement(int index, ConditionalExpression cond)
        : cond(cond), Statement(index, StatementType::IF) {
        this->thenStmtLst = {};
        this->elseStmtLst = {};
    }

    void addThenStatement(Statement statement) {
        this->thenStmtLst.push_back(statement);
    }
    void addElseStatement(Statement statement) {
        this->elseStmtLst.push_back(statement);
    }

    ConditionalExpression getConditional() { return this->cond; }

    vector<Statement> getThenStmtLst() { return this->thenStmtLst; }

    vector<Statement> getElseStmtLst() { return this->elseStmtLst; }
};
