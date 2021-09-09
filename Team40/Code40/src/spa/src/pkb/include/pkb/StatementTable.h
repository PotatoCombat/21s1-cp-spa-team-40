#pragma once

#include "Abstractions.h"
#include "Iterator.h"

#include <map>
#include <vector>

using namespace std;

template <typename T, typename Index> class StatementTable {
    static_assert(is_convertible<int, Index>::value,
                  "Index must be convertible to a primitive int");

public:
    StatementTable();

    /// Stores the pointer \param stmt in the table, and indexes the statement.
    /// \return index of the \param stmt in the table.
    Index insert(T *stmt);

    /// Returns the pointer to the statement stored at \param index in the table.
    /// \return pointer, or NULL if the table does not contain \param index.
    T *getStmt(Index index);

    /// Returns the index of \param stmt in the table.
    /// \return index, or InvalidIndex if the table does not contain \param stmt.
    Index getIndex(T *stmt);

    /// Returns an iterator for all the statement indices stored in the table.
    Iterator<Index> getIndices();

    /// Returns the number of statements stored in the table.
    int getSize();

private:
    Index size = 0;
    vector<T *> statements;
    vector<Index> indices;
    map<T *, Index> statementToIndex;
};

template <typename T, typename Index>
StatementTable<T, Index>::StatementTable() = default;

template <typename T, typename Index>
Index StatementTable<T, Index>::insert(T *stmt) {
    size++;
    statements.push_back(stmt);
    indices.push_back(size);
    statementToIndex[stmt] = size;
    return size;
}

template <typename T, typename Index>
T *StatementTable<T, Index>::getStmt(Index index) {
    if (index > size)
    {
        return NULL;
    }
    return statements.at(index - 1);
}

template <typename T, typename Index>
Index StatementTable<T, Index>::getIndex(T *stmt) {
    auto search = statementToIndex.find(stmt);
    if (search == statementToIndex.end())
    {
        return InvalidIndex;
    }
    return search->second;
}

template <typename T, typename Index>
Iterator<Index> StatementTable<T, Index>::getIndices() {
    return Iterator<Index>(indices);
}

template <typename T, typename Index> int StatementTable<T, Index>::getSize() {
    return size;
}
