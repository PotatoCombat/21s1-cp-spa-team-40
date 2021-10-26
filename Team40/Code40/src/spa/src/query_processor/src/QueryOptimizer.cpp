#include "query_processor/QueryOptimizer.h"

void QueryOptimizer::optimize(Query& before, Query& after) {
    int INVALID_IDX = -1;
    int NUMBER_OF_CLS_IN_ONE_CLS = 1;
    vector<Clause *> unsortedClauses = before.getClauses();
    vector<Reference *> refs = before.getReferences();

    int totalGroup = 0;
    map<string, int> refToGroup;

    for (auto ref : refs) {
        refToGroup[ref->getValue()] = INVALID_IDX;
    }

    vector<pair<int, int>> groupSizes; // first value is group idx, second value is groupSize

    // form groups
    for (Clause *cls : unsortedClauses) {
        Reference *ref1 = cls->getFirstReference();
        Reference *ref2 = cls->getSecondReference();

        bool isSynonym1 = ref1->getRefType() == ReferenceType::SYNONYM;
        bool isSynonym2 = ref2->getRefType() == ReferenceType::SYNONYM;

        string val1 = ref1->getValue();
        string val2 = ref2->getValue();

        int idx1 = INVALID_IDX;
        int idx2 = INVALID_IDX;

        int groupIdx = INVALID_IDX;
        
        // skip clauses that return boolean
        if (!isSynonym1 && !isSynonym2) {
            continue;
        }

        if (isSynonym1) {
            idx1 = refToGroup[val1];
            groupIdx = max(groupIdx, idx1);
        }

        if (isSynonym2) {
            idx2 = refToGroup[val2];
            groupIdx = max(groupIdx, idx2);
        }

        if (groupIdx == INVALID_IDX) {
            groupIdx = totalGroup;
            groupSizes.push_back({groupIdx, NUMBER_OF_CLS_IN_ONE_CLS});
            totalGroup++;
        } else {
            if (idx1 != INVALID_IDX && idx2 != INVALID_IDX) {
                pair<int, int> &g1 = groupSizes[idx1];
                pair<int, int> &g2 = groupSizes[idx2];
                if (idx1 != groupIdx) {
                    g2.second += g1.second + NUMBER_OF_CLS_IN_ONE_CLS;
                    g1.second = 0;
                } else if (idx2 != groupIdx) {
                    g1.second += g2.second + NUMBER_OF_CLS_IN_ONE_CLS;
                    g2.second = 0;
                } else { // g1 = g2 = groupIdx
                    g1.second++;
                }
            }
        }

        // merge 2 groups
        for (auto &kvp : refToGroup) { // kvp: key value pair
            if (isSynonym1 && kvp.first == val1) {
                kvp.second = groupIdx;
                continue;
            }
            
            if (isSynonym2 && kvp.first == val2) {
                kvp.second = groupIdx;
                continue;
            }
            
            int group = kvp.second;
            if (group != INVALID_IDX && (group == idx1 || group == idx2)) {
                kvp.second = groupIdx;
            }
        }
    }

    // sort groups by ascending group size
    sort(groupSizes.begin(), groupSizes.end(), 
        [](const pair<int, int> &a, const pair<int, int> &b) {
            return a.second < b.second; 
        });

    // create new clause list
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

    // populate new query object
    for (auto cls : sortedClauses) {
        after.addClause(cls);
    }
    for (auto ref : before.getReturnReferences()) {
        after.addReturnReference(ref);
    }
}
