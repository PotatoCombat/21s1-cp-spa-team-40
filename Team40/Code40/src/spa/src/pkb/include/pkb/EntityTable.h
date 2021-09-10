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
    /// Stores the pointer \param entity in the table, and indexes the entity.
    /// \return index of the \param entity in the table.
    virtual Index insert(T *entity) {
        entities.push_back(entity);
        indices.push_back(size);
        entityToIndex[entity] = size;
        return size++;
    }

    /// Returns the pointer to the entity stored at \param index in the table.
    /// \return pointer, or NULL if the table does not contain \param index.
    virtual T *getEntity(Index index) {
        if (index >= size)
        {
            return NULL;
        }
        return entities.at(index);
    }

    /// Returns the index of \param entity in the table.
    /// \return index, or InvalidIndex if the table does not contain \param entity.
    virtual Index getIndex(T *entity) {
        auto search = entityToIndex.find(entity);
        if (search == entityToIndex.end())
        {
            return InvalidIndex;
        }
        return search->second;
    }

    /// Returns an iterator for all the entity indices stored in the table.
    virtual Iterator<Index> getIndices() {
        return Iterator<Index>(indices);
    }

    /// Returns the number of entities stored in the table.
    virtual int getSize() {
        return size;
    }

private:
    Index size = 0;
    vector<T *> entities;
    vector<Index> indices;
    map<T *, Index> entityToIndex;
};
