#include "query_processor/QueryOptimizer.h"

void QueryOptimizer::optimize(Query& before, Query& after) {
    
    vector<Clause *> unsortedClauses = before.getClauses();

    map<string, int> refToGroup;

    for (auto ref : before.getReferences()) {
        refToGroup[ref->getValue()] = INVALID_IDX;
    }
    
    // form groups
    vector<pair<int, int>> groupSizes = formGroups(unsortedClauses, refToGroup);


    // sort groups by ascending group size
    sort(groupSizes.begin(), groupSizes.end(), 
        [](const pair<int, int> &a, const pair<int, int> &b) {
            return a.second < b.second; 
        });

    // create new clause list
    vector<Clause *> sortedClauses = reorderClause(unsortedClauses, groupSizes, refToGroup);

    // populate new query object
    populateNewQueryObject(sortedClauses, before, after);
}

vector<pair<int, int>>
QueryOptimizer::formGroups(vector<Clause *> &unsortedClauses,
                           map<string, int> &refToGroup) {
    vector<pair<int, int>> groupSizes;

    for (Clause *cls : unsortedClauses) {
        vector<int> indexes = updateGroupSize(cls, groupSizes, refToGroup);
        if (indexes.size() != 3) {
            throw runtime_error("invalid number of return values");
        }
        int idx1 = indexes[0];
        int idx2 = indexes[1];
        int newGroupIdx = indexes[2];
        if (newGroupIdx == INVALID_IDX) {
            continue;
        }
        mergeTwoGroups(newGroupIdx, idx1, idx2, cls, refToGroup);
    }

    return groupSizes;
}

vector<int> QueryOptimizer::updateGroupSize(Clause *cls,
                                     vector<pair<int, int>> &groupSizes,
                                     map<string, int> &refToGroup) {
    int NUMBER_OF_CLS_IN_ONE_CLS = 1;

    Reference *ref1 = cls->getFirstReference();
    Reference *ref2 = cls->getSecondReference();

    bool isSynonym1 = ref1->getRefType() == ReferenceType::SYNONYM;
    bool isSynonym2 = ref2->getRefType() == ReferenceType::SYNONYM;

    string val1 = ref1->getValue();
    string val2 = ref2->getValue();

    int idx1 = INVALID_IDX;
    int idx2 = INVALID_IDX;

    int newGroupIdx = INVALID_IDX;

    // skip clauses that return boolean
    if (!isSynonym1 && !isSynonym2) {
        return {idx1, idx2, newGroupIdx};
    }

    if (isSynonym1) {
        idx1 = refToGroup[val1];
        newGroupIdx = max(newGroupIdx, idx1);
    }

    if (isSynonym2) {
        idx2 = refToGroup[val2];
        newGroupIdx = max(newGroupIdx, idx2);
    }

    if (newGroupIdx == INVALID_IDX) {
        newGroupIdx = groupSizes.size();
        groupSizes.push_back({newGroupIdx, NUMBER_OF_CLS_IN_ONE_CLS});
        return {idx1, idx2, newGroupIdx};
    }

    if (idx1 == INVALID_IDX || idx2 == INVALID_IDX) {
        return {idx1, idx2, newGroupIdx};
    }

    if (idx1 != INVALID_IDX && idx2 != INVALID_IDX) {
        pair<int, int> &g1 = groupSizes[idx1]; // group1
        pair<int, int> &g2 = groupSizes[idx2]; // group2
        if (idx1 != newGroupIdx) {             // merge into group2
            g2.second += g1.second + NUMBER_OF_CLS_IN_ONE_CLS;
            g1.second = 0;
        } else if (idx2 != newGroupIdx) { // merge into group1
            g1.second += g2.second + NUMBER_OF_CLS_IN_ONE_CLS;
            g2.second = 0;
        } else { // group1 and group2 are the same
            g1.second++;
        }
    }

    return {idx1, idx2, newGroupIdx};
}

void QueryOptimizer::mergeTwoGroups(int newGroupIdx, int idx1, int idx2,
                                    Clause *cls, map<string, int> &refToGroup) {
    Reference *ref1 = cls->getFirstReference();
    Reference *ref2 = cls->getSecondReference();

    bool isSynonym1 = ref1->getRefType() == ReferenceType::SYNONYM;
    bool isSynonym2 = ref2->getRefType() == ReferenceType::SYNONYM;

    string val1 = ref1->getValue();
    string val2 = ref2->getValue();

    for (auto &kvp : refToGroup) { // kvp: key value pair
        if (isSynonym1 && kvp.first == val1) {
            kvp.second = newGroupIdx;
            continue;
        }

        if (isSynonym2 && kvp.first == val2) {
            kvp.second = newGroupIdx;
            continue;
        }

        int group = kvp.second;
        if (group != INVALID_IDX && (group == idx1 || group == idx2)) {
            kvp.second = newGroupIdx;
        }
    }
}

vector<Clause *>
QueryOptimizer::reorderClause(vector<Clause *> &unsortedClauses,
                              vector<pair<int, int>> &groupSizes,
                              map<string, int> &refToGroup) {
    vector<Clause *> sortedClauses;

    // prioritize boolean return values
    for (Clause *cls : unsortedClauses) {
        ReferenceType refType1 = cls->getFirstReference()->getRefType();
        ReferenceType refType2 = cls->getSecondReference()->getRefType();
        if (refType1 != ReferenceType::SYNONYM &&
            refType2 != ReferenceType::SYNONYM) {
            sortedClauses.push_back(cls);
        }
    }

    for (auto &kvp : groupSizes) {
        int groupIdx = kvp.first;
        for (Clause *cls : unsortedClauses) {
            Reference *ref1 = cls->getFirstReference();
            Reference *ref2 = cls->getSecondReference();

            ReferenceType refType1 = ref1->getRefType();
            ReferenceType refType2 = ref2->getRefType();

            string val1 = ref1->getValue();
            string val2 = ref2->getValue();

            bool match = false;
            if (refType1 == ReferenceType::SYNONYM &&
                refToGroup[val1] == groupIdx) {
                match = true;
            }
            if (refType2 == ReferenceType::SYNONYM &&
                refToGroup[val2] == groupIdx) {
                match = true;
            }

            if (match) {
                sortedClauses.push_back(cls);
            }
        }
    }
    return sortedClauses;
}

void QueryOptimizer::populateNewQueryObject(vector<Clause*> sortedClauses, Query& before, Query& after) {
    for (auto cls : sortedClauses) {
        after.addClause(cls);
    }
    for (auto ref : before.getReturnReferences()) {
        after.addReturnReference(ref);
    }
}
