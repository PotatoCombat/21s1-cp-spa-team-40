#pragma once

#include <map>
#include <set>
#include <vector>

#include "Iterator.h"

#include "common/model/Abstractions.h"
#include "common/model/Entity.h"

using namespace std;

template <typename T, typename Name>
class EntityTable {
    static_assert(is_convertible<T, Entity<Name>>::value,
            "Cannot use table with an invalid entity type");

public:
    /**
     * Stores an entity in the table.
     * @param entity
     */
    virtual void insert(T *entity) {
        size++;
        names.insert(entity->getName());
        nameToEntities[entity->getName()] = entity;
    }

    /**
     * Returns an entity in the table.
     * @param name the name of the entity.
     * @returns the statement with the given index,
     *          or InvalidEntity if none exist.
     */
    virtual T *getEntity(const Name &name) {
        auto key = nameToEntities.find(name);
        if (key == nameToEntities.end()) {
            return nullptr;
        }
        return key->second;
    }

    /**
     * Returns a list of entity names in the table.
     */
    virtual Iterator<Name> getNames() { return Iterator<Name>(names); }

    /**
     * Returns the number of entities stored in the table.
     */
    virtual size_t getSize() { return size; }

private:
    size_t size = 0;
    set<Name> names;
    map<Name, T *> nameToEntities;
};
