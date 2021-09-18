#pragma once
#include <map>
#include <set>
#include <vector>
#include "Iterator.h"
#include "common/model/Abstractions.h"

using namespace std;

template <typename T, typename Name> class EntityTable {
    static_assert(is_convertible<string, Name>::value,
                  "Name must be convertible to a primitive string");

public:
    /// Stores the pointer \param entity in the map.
    virtual void insert(T *entity) {
        string name = entity->getName();
        names.insert(name);
//        entities.push_back(entity);
        nameToEntityMap[name] = entity;
        ++size;
    }

    /// Returns the pointer to the entity with the given name in the map.
    /// \return pointer, or NULL if the table does not contain \param index.
    virtual T *getEntity(Name name) {
        auto result = nameToEntityMap.find(name);
        if (result == nameToEntityMap.end()) {
            return nullptr;
        }
        return result->second;
    }

    /// Returns an iterator for all the entity indices stored in the table.
    virtual Iterator<Name> getNames() { return Iterator<Name>(names); }

    /// Returns the number of entities stored in the table.
    virtual int getSize() { return size; }

private:
    int size = 0;
    set<Name> names;
//    vector<T*> entities;
    map<Name, T *> nameToEntityMap;
};
