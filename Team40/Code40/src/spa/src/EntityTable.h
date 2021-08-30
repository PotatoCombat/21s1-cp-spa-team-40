#pragma once

#include <map>
#include <vector>

using namespace std;

template <typename T, typename U>
class EntityTable {
    static_assert(is_convertible<int, U>::value, "U must be convertible to a primitive int");

public:
    EntityTable();
    ~EntityTable();

    U insert(T entity);
    U getIndex(T entity);
    T getEntity(U index);
    const vector<T> &getEntities() const;
    int getSize();

private:
    U size = 0;
    vector<T> indexToEntity;
    map<T, U> entityToIndex;
};

template <typename T, typename U>
EntityTable<T, U>::EntityTable() = default;

template <typename T, typename U>
EntityTable<T, U>::~EntityTable() = default;

template <typename T, typename U>
U EntityTable<T, U>::insert(T entity) {
    indexToEntity.push_back(entity);
    entityToIndex[entity] = size;
    return size++;
}

template <typename T, typename U>
U EntityTable<T, U>::getIndex(T entity) {
    return entityToIndex.at(entity);
}

template <typename T, typename U>
T EntityTable<T, U>::getEntity(U index) {
    return indexToEntity.at(index);
}

template <typename T, typename U>
const vector<T> &EntityTable<T, U>::getEntities() const {
    return indexToEntity;
}

template <typename T, typename U>
int EntityTable<T, U>::getSize() {
    return size;
}
