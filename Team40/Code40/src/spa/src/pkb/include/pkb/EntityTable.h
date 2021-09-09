#pragma once

#include "Abstractions.h"
#include "Iterator.h"

#include <map>
#include <vector>

using namespace std;

template <typename T, typename Index> class EntityTable {
    static_assert(is_convertible<int, Index>::value,
                  "Index must be convertible to a primitive int");

public:
    EntityTable();

    /// Stores the pointer \param entity in the table, and indexes the entity.
    /// \return index of the \param entity in the table.
    Index insert(T *entity);

    /// Returns the pointer to the entity stored at \param index in the table.
    /// \return pointer, or NULL if the table does not contain \param index.
    T *getEntity(Index index);

    /// Returns the index of \param entity in the table.
    /// \return index, or InvalidIndex if the table does not contain \param entity.
    Index getIndex(T *entity);

    /// Returns an iterator for all the entity indices stored in the table.
    Iterator<Index> getIndices();

    /// Returns the number of entities stored in the table.
    int getSize();

private:
    Index size = 0;
    vector<T *> entities;
    vector<Index> indices;
    map<T *, Index> entityToIndex;
};

template <typename T, typename Index>
EntityTable<T, Index>::EntityTable() = default;

template <typename T, typename Index>
Index EntityTable<T, Index>::insert(T *entity) {
    entities.push_back(entity);
    indices.push_back(size);
    entityToIndex[entity] = size;
    return size++;
}

template <typename T, typename Index>
T *EntityTable<T, Index>::getEntity(Index index) {
    if (index >= size)
    {
        return NULL;
    }
    return entities.at(index);
}

template <typename T, typename Index>
Index EntityTable<T, Index>::getIndex(T* entity) {
    auto search = entityToIndex.find(entity);
    if (search == entityToIndex.end())
    {
        return InvalidIndex;
    }
    return search->second;
}

template <typename T, typename Index>
Iterator<Index> EntityTable<T, Index>::getIndices() {
    return Iterator<Index>(indices);
}

template <typename T, typename Index> int EntityTable<T, Index>::getSize() {
    return size;
}
