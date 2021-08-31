#include "Statement.h"
#include <vector>

class IfStatement : public Statement {
private:
    // ConditionalExpression cond; //TODO
    vector<Statement> stmtLst;

public:
    IfStatement::IfStatement(int index, ConditionalExpression cond)
        : cond(cond), Statement(index, StatementType::IF) {
            this->stmtLst = {};
        }

    void addStatement(Statement statement) {
        this->stmtLst.push_back(statement);
    }

    ConditionalExpression getConditional() { return this->cond; }

    vector<Statement> getStmtLst() { return this->stmtLst; }
};
