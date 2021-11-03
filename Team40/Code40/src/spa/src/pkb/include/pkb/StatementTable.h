#pragma once

#include <map>
#include <vector>

#include "Iterator.h"
#include "common/model/Abstractions.h"
#include "common/model/Statement.h"

using namespace std;

class StatementTable {
public:
    StatementTable();

    /**
     * Stores a statement in the table.
     * @param stmt the pointer to the statement.
     */
    void insert(Statement *stmt);

    /**
     * Returns a statement in the table.
     * @param index the statement number of the statement.
     * @returns the statement with the given index,
     *          or InvalidStatement if none exist.
     */
    Statement *getStmt(const StmtIndex &index);

    /**
     * Returns the type of a statement in the table.
     * @param index the statement number of the statement.
     * @returns the type of the statement with the given index,
     *          or StatementType::UNKNOWN if none exist.
     */
    StatementType getStmtType(const StmtIndex &index);

    /**
     * Returns a list of statement numbers in the table.
     */
    Iterator<StmtIndex> getIndices();

    /**
     * Returns a list of statement numbers in the table,
     * filtered by the given statement type.
     * @param type the type of the statements to filter.
     */
    Iterator<StmtIndex> getIndices(const StatementType &type);

    /**
     * Returns the number of statements stored in the table.
     */
    size_t getSize() const;

private:
    size_t size = 0;

    vector<Statement *> statements;
    vector<StmtIndex> indices;

    map<StatementType, vector<StmtIndex>> indicesByType = {
            { StatementType::ASSIGN, { } },
            { StatementType::READ, { } },
            { StatementType::PRINT, { } },
            { StatementType::IF, { } },
            { StatementType::WHILE, { } },
            { StatementType::CALL, { } },
    };
};
