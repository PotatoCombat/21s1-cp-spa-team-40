#pragma once

#include "Abstractions.h"
#include "Iterator.h"

#include "common/model/statement/Statement.h"

#include <map>
#include <vector>

using namespace std;

class StatementTable {
public:
    StatementTable();

    /// Stores the pointer \param stmt in the table, and indexes the statement.
    /// \return index of the \param stmt in the table.
    StmtIndex insert(Statement *stmt);

    /// Returns the pointer to the statement stored at \param index in the table.
    /// \return pointer, or NULL if the table does not contain \param index.
    Statement *getStmt(StmtIndex index);

    /// Returns the pointer to the statement stored at \param index in the table.
    /// \return pointer, or NULL if the table does not contain \param index.
    StatementType getStmtType(StmtIndex index);

    /// Returns the index of \param stmt in the table.
    /// \return index, or InvalidIndex if the table does not contain \param stmt.
    StmtIndex getIndex(Statement *stmt);

    /// Returns an iterator for all the statement indices stored in the table.
    Iterator<StmtIndex> getIndices();

    Iterator<StmtIndex> getIndices(StatementType type);

    /// Returns the number of statements stored in the table.
    int getSize();

private:
    StmtIndex size = 0;
    vector<Statement *> statements;
    vector<StmtIndex> indices;
    map<Statement *, StmtIndex> statementToIndex;

    vector<StmtIndex> assignStatements;
    vector<StmtIndex> readStatements;
    vector<StmtIndex> printStatements;
    vector<StmtIndex> ifStatements;
    vector<StmtIndex> whileStatements;
    vector<StmtIndex> callStatements;
};
