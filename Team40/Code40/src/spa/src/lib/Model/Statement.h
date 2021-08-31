#pragma once
#include <vector>
using namespace std;

enum class StatementType { READ, PRINT, ASSIGN, CALL, WHILE, IF };

class Statement {
private:
    StatementType statementType;
    int index;
    vector<char> varsList;
    vector<int> constsList;

public:
    // constructor
    Statement(int index);

    // setters
    void setStatementType(StatementType statementType);
    void addVar(char var);
    void addConst(int const);

    // getters
    int getIndex();
    StatementType getStatementType();
    vector<char> getVars();
    vector<int> getConsts();
};
