#include "Statement.h"
#include "../conditional/Conditional.h"
#include <vector>

using namespace std;
class IfStatement : public Statement {
private:
    Conditional cond;
    vector<Statement> stmtLst;

public:
    // constructor
    IfStatement(int index, Conditional cond)
        : cond(cond), Statement(index, StatementType::IF) {
        this->stmtLst = {};
    }

    // adder
    void addStatement(Statement statement) {
        this->stmtLst.push_back(statement);
    }

    // getters
    Conditional getConditional() { return this->cond; }

    vector<Statement> getStmtLst() { return this->stmtLst; }
};
