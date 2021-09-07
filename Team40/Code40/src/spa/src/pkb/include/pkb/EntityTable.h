#pragma once

#include "Iterator.h"

#include <map>
#include <vector>

using namespace std;

template <typename T, typename Index>
class EntityTable {
    static_assert(is_convertible<int, Index>::value, "Index must be convertible to a primitive int");

public:
    EntityTable();

    int getSize();

    T getEntity(Index index);
    Index getIndex(T entity);
    Iterator<Index> getIndices();

    Index insert(T entity);

private:
    Index size = 0;
    vector<T> entities;
    vector<Index> indices;
    map<T, Index> entityToIndex;
};

template <typename T, typename Index>
EntityTable<T, Index>::EntityTable() = default;

template <typename T, typename Index>
int EntityTable<T, Index>::getSize() {
    return size;
}

template <typename T, typename Index>
T EntityTable<T, Index>::getEntity(Index index) {
    return entities.at(index);
}

template <typename T, typename Index>
Index EntityTable<T, Index>::getIndex(T entity) {
    return entityToIndex.at(entity);
}

template <typename T, typename Index>
Iterator<Index> EntityTable<T, Index>::getIndices() {
    return Iterator<Index>(indices);
}

template <typename T, typename Index>
Index EntityTable<T, Index>::insert(T entity) {
    entities.push_back(entity);
    indices.push_back(size);
    entityToIndex[entity] = size;
    return size++;
}
