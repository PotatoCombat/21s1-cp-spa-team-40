#pragma once

enum class StatementType { READ, PRINT, ASSIGN, CALL, WHILE, IF, NONE };

class Statement {
private:
    StatementType statementType;
    int index;

protected:
public:
    // constructor
    Statement(int index, StatementType statementType);
    Statement(StatementType statementType);
    Statement();

    // getters
    int getIndex();
    StatementType getStatementType();
};
