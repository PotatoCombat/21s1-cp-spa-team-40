#include "Statement.h"
#include "factor/Conditional.h"
#include <vector>

class IfStatement : public Statement {
private:
    Conditional cond; //TODO
    vector<Statement> thenStmtLst;
    vector<Statement> elseStmtLst;

public:
    IfStatement::IfStatement(int index, Conditional cond)
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

    Conditional getConditional() { return this->cond; }

    vector<Statement> getThenStmtLst() { return this->thenStmtLst; }

    vector<Statement> getElseStmtLst() { return this->elseStmtLst; }
};
