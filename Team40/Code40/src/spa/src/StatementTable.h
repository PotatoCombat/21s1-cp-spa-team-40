#pragma once

#include <map>
#include <vector>

using namespace std;

template <typename T, typename Index>
class StatementTable {
    static_assert(is_convertible<int, Index>::value, "Index must be convertible to a primitive int");

public:
    StatementTable();

    int getSize();
    Index insert(T stmt);
    T getStmt(Index index);
    Index getIndex(T stmt);
    const vector<Index> &getIndices();

private:
    Index size = 0;
    vector<T> statements;
    vector<Index> indices;
    map<T, Index> statementToIndex;
};

template <typename T, typename Index>
StatementTable<T, Index>::StatementTable() = default;

template <typename T, typename Index>
int StatementTable<T, Index>::getSize() {
    return size;
}

template <typename T, typename Index>
Index StatementTable<T, Index>::insert(T stmt) {
    size++;
    statements.push_back(stmt);
    indices.push_back(size);
    statementToIndex[stmt] = size;
    return size;
}

template <typename T, typename Index>
T StatementTable<T, Index>::getStmt(Index index) {
    return statements.at(index - 1);
}

template <typename T, typename Index>
Index StatementTable<T, Index>::getIndex(T stmt) {
    return statementToIndex.at(stmt);
}

template <typename T, typename Index>
const vector<Index> &StatementTable<T, Index>::getIndices() {
    return indices;
}
