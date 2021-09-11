#include "common/model/statement/Statement.h"
#include <string>
#include <vector>
using namespace std;

class IfStatement : public Statement {
private:
    Condition *cond;
    vector<Statement *> thenStmtLst;
    vector<Statement *> elseStmtLst;
    vector<string> condLst;

public:
    IfStatement(int index, Condition *cond);

    IfStatement(int index);

    void addThenStatement(Statement *statement);
    void addElseStatement(Statement *statement);

    void setCondLst(vector<string> condLst);

    Condition *getCondition() override;

    vector<Statement *> getThenStmtLst() override;

    vector<Statement *> getElseStmtLst() override;
};
