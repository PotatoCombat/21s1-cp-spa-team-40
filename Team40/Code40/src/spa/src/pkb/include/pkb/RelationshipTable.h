#pragma once

#include <map>
#include <set>

template <typename L, typename R>
class RelationshipTable {
public:
    inline RelationshipTable() = default;

    /**
     * Stores Relationship(lhs, rhs).
     */
    inline void insertRelationship(const L &lhs, const R &rhs) {
        insertForward(lhs, rhs);
        insertBackward(lhs, rhs);
    };

    /**
     * Updates transitive relationships for Relationship(lhs, rhs).
     */
    inline void updateTransitivity(const L &lhs, const R &rhs) {
        // Adding LHS transitivity
        for (const L &transitiveLHS : getLHSRelationships(lhs)) {
            insertForward(transitiveLHS, rhs);
            insertBackward(transitiveLHS, rhs);
        }

        // Adding RHS transitivity
        for (const R &transitiveRHS : getRHSRelationships(rhs)) {
            insertForward(lhs, transitiveRHS);
            insertBackward(lhs, transitiveRHS);
        }
    };

    /**
     * Selects lhs such that Relationship(lhs, rhs),
     * where lhs is the first entry inserted into the table.
     * @param invalidValue returns this value if there are no
     *                     entries for lhs.
     */
    inline L getFirstLHSRelationship(const R &rhs, const L &invalidValue) {
        auto key = backwardFirstEntryMap.find(rhs);
        if (key == backwardFirstEntryMap.end())
        {
            return invalidValue;
        }
        return key->second;
    };

    /**
     * Selects rhs such that Relationship(lhs, rhs),
     * where rhs is the first entry inserted into the table.
     * @param invalidValue returns this value if there are no
     *                     entries for rhs.
     */
    inline R getFirstRHSRelationship(const L &lhs, const R &invalidValue) {
        auto key = forwardFirstEntryMap.find(lhs);
        if (key == forwardFirstEntryMap.end())
        {
            return invalidValue;
        }
        return key->second;
    };

    /**
     * Selects lhs such that Relationship(lhs, rhs).
     */
    inline set<L> getLHSRelationships(const R &rhs) {
        auto key = backwardEntriesMap.find(rhs);
        if (key == backwardEntriesMap.end())
        {
            return { };
        }
        return key->second;
    };

    /**
     * Selects rhs such that Relationship(lhs, rhs).
     */
    inline set<R> getRHSRelationships(const L &lhs) {
        auto key = forwardEntriesMap.find(lhs);
        if (key == forwardEntriesMap.end())
        {
            return { };
        }
        return key->second;
    };

    /**
     * Selects BOOLEAN such that Relationship(lhs, rhs).
     */
    inline bool isRelationship(const L &lhs, const R &rhs) {
        auto key = backwardEntriesMap.find(rhs);
        if (key == backwardEntriesMap.end())
        {
            return false;
        }
        auto values = key->second;
        return values.find(lhs) != values.end();
    };

private:
    /**
     * Maps lhs -> first rhs
     */
    map<L, R> forwardFirstEntryMap;

    /**
     * Maps rhs -> first lhs
     */
    map<R, L> backwardFirstEntryMap;

    /**
     * Maps lhs -> all rhs
     */
    map<L, set<R>> forwardEntriesMap;

    /**
     * Maps rhs -> all lhs
     */
    map<R, set<L>> backwardEntriesMap;

    /**
     * Stores a forward map entry.
     */
    inline void insertForward(const L &lhs, const R &rhs) {
        auto key = forwardEntriesMap.find(lhs);
        if (key == forwardEntriesMap.end()) {
            forwardFirstEntryMap[lhs] = rhs;
            forwardEntriesMap[lhs] = { rhs };
        }
        else {
            key->second.insert(rhs);
        }
    };

    /**
     * Stores a backward map entry.
     */
    inline void insertBackward(const L &lhs, const R &rhs) {
        auto key = backwardEntriesMap.find(rhs);
        if (key == backwardEntriesMap.end()) {
            backwardFirstEntryMap[rhs] = lhs;
            backwardEntriesMap[rhs] = { lhs };
        }
        else {
            key->second.insert(lhs);
        }
    };
};
