#pragma once

enum class StatementType { READ, PRINT, ASSIGN, CALL, WHILE, IF };

class Statement {
private:
    StatementType statementType;
    int index;

protected:
public:
    // constructor
    Statement(int index, StatementType statementType);

    // getters
    int getIndex();
    StatementType getStatementType();
};
