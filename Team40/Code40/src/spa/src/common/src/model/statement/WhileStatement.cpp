#include "Statement.h"
#include "Conditional.h"
#include <string>
#include <vector>

using namespace std;
class WhileStatement : public Statement {
private:
    // No AST yet
    // Condition cond;
    vector<Statement> stmtLst;
    vector<string> condLst;

public:
    // constructor
    // WhileStatement(int index, Condition cond)
    //     : cond(cond), Statement(index, StatementType::WHILE) {
    //     this->stmtLst = {};
    // }

    WhileStatement(int index) : Statement(index, StatementType::WHILE) {
        this->stmtLst = {};
        this->condLst = {};
    }

    // adder
    void addStatement(Statement statement) {
        this->stmtLst.push_back(statement);
    }

    void setCondLst(vector<string> condLst) { this->condLst = condLst; }

    // getters
    // Condition getCondition() { return this->cond; }

    vector<Statement> getStmtLst() { return this->stmtLst; }
};
