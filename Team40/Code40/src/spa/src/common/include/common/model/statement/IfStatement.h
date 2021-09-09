#include "common/model/statement/Statement.h"
#include <string>
#include <vector>
using namespace std;

class IfStatement : public Statement {
private:
    Condition *cond;
    vector<Statement> thenStmtLst;
    vector<Statement> elseStmtLst;
    vector<string> condLst;

public:
    IfStatement(int index, Condition *cond)
        : cond(cond), Statement(index, StatementType::IF) {
        this->thenStmtLst = {};
        this->elseStmtLst = {};
    }

    IfStatement(int index) : Statement(index, StatementType::IF) {
        this->thenStmtLst = {};
        this->elseStmtLst = {};
        this->condLst = {};
    }

    void addThenStatement(Statement statement) {
        this->thenStmtLst.push_back(statement);
    }
    void addElseStatement(Statement statement) {
        this->elseStmtLst.push_back(statement);
    }

    void setCondLst(vector<string> condLst) { condLst = condLst; }

    Condition *getCondition() { return cond; }

    vector<Statement> getThenStmtLst() { return thenStmtLst; }

    vector<Statement> getElseStmtLst() { return elseStmtLst; }
};
