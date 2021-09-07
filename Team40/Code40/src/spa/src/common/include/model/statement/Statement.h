#pragma once

enum class StatementType { READ, PRINT, ASSIGN, CALL, WHILE, IF };

class Statement {
private:
    StatementType statementType;
    int index;

protected:
    // constructor
    Statement(int index, StatementType statementType);

public:
    // getters
    int getIndex();
    StatementType getStatementType();
};
