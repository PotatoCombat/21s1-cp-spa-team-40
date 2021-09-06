#include "../condition/Condition.h"
#include "Statement.h"
#include <vector>

using namespace std;
class WhileStatement : public Statement {
private:
    Condition cond;
    vector<Statement> stmtLst;

public:
    // constructor
    WhileStatement(int index, Condition cond)
        : cond(cond), Statement(index, StatementType::WHILE) {
        this->stmtLst = {};
    }

    // adder
    void addStatement(Statement statement) {
        this->stmtLst.push_back(statement);
    }

    // getters
    Condition getCondition() { return this->cond; }

    vector<Statement> getStmtLst() { return this->stmtLst; }
};
