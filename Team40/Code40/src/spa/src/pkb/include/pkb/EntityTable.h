#pragma once

#include <map>
#include <set>
#include <vector>

#include "pkb/Iterator.h"

#include "common/model/Abstractions.h"
#include "common/model/Entity.h"

using namespace std;

template <typename T, typename Id>
class EntityTable {
    static_assert(is_convertible<T, Entity<Id>>::value,
            "Cannot use table with an invalid entity type");

public:
    /**
     * Stores an entity in the table.
     * @param entity
     */
    virtual void insert(T *entity) {
        size++;
        ids.insert(entity->getId());
        idToEntities[entity->getId()] = entity;
    }

    /**
     * Returns an entity in the table.
     * @param name the name of the entity.
     * @returns the statement with the given index,
     *          or InvalidEntity if none exist.
     */
    virtual T *getEntity(const Id &id) {
        auto key = idToEntities.find(id);
        if (key == idToEntities.end()) {
            return nullptr;
        }
        return key->second;
    }

    /**
     * Returns a list of entity names in the table.
     */
    virtual Iterator<Id> getIds() { return Iterator<Id>(ids); }

    /**
     * Returns the number of entities stored in the table.
     */
    virtual size_t getSize() { return size; }

private:
    size_t size = 0;
    set<Id> ids;
    map<Id, T *> idToEntities;
};
