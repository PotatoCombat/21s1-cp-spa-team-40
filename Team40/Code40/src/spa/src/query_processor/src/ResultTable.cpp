#include "query_processor/ResultTable.h"

void ResultTable::clear() { table.clear(); }

void ResultTable::init(int size) {
    vector<VALUE_TO_POINTERS_MAP> temp(size, VALUE_TO_POINTERS_MAP{});
    table = temp;
}

void ResultTable::addValues(INDEX sourceIdx, VALUE sourceVal, INDEX targetIdx, VALUE_SET targetVals) {
    assertIndex(sourceIdx);
    assertIndex(targetIdx);

    VALUE_TO_POINTERS_MAP *map = &table[sourceIdx];
    if (map->find(sourceVal) == map->end()) {
        (*map)[sourceVal] = IDX_TO_VALUES_MAP{{targetIdx, targetVals}};
        return;
    }
    
    IDX_TO_VALUES_MAP *map2 = &(*map)[sourceVal];
    if (map2->find(targetIdx) == map2->end()) {
        (*map2)[targetIdx] = targetVals;
        return;
    }

    (*map2)[targetIdx].insert(targetVals.begin(), targetVals.end());
}

VALUE_SET ResultTable::getLinkedValues(INDEX sourceIdx, VALUE value, INDEX targetIdx) {
    assertIndex(sourceIdx);
    assertIndex(targetIdx);
    VALUE_TO_POINTERS_MAP *map = &table[sourceIdx];
    if (map->find(value) == map->end()) {
        return VALUE_SET{};
    }

    IDX_TO_VALUES_MAP *pIndexToValue = &(*map)[value];
    if (pIndexToValue->find(targetIdx) == pIndexToValue->end()) {
        return VALUE_SET{};
    }
    return (*pIndexToValue)[targetIdx];
}

bool ResultTable::hasPointerToIdx(int sourceIdx, string sourceValue,
                     int targetIdx) {
    assertIndex(sourceIdx);
    assertIndex(targetIdx);

    VALUE_TO_POINTERS_MAP *map = &table[sourceIdx];
    if (map->find(sourceValue) == map->end()) {
        return false;
    }

    IDX_TO_VALUES_MAP *pIndexToValue = &(*map)[sourceValue];
    if (pIndexToValue->find(targetIdx) != pIndexToValue->end()) {
        return false;
    }

    return pIndexToValue[targetIdx].size() > 0;
}

void ResultTable::removeValue(INDEX refIndex, VALUE value) {
    set<pair<INDEX, VALUE>> visited;
    vector<pair<INDEX, VALUE>> toRemove{make_pair(refIndex, value)};
    while (!toRemove.empty()) {
        pair<INDEX, VALUE> ref = toRemove.back();
        toRemove.pop_back();
        if (visited.find(ref) != visited.end()) {
            continue;
        }
        visited.insert(ref);
        INDEX sourceIdx = ref.first;
        VALUE sourceVal = ref.second;

        for (auto &idxToValuesPair : table[sourceIdx][sourceVal]) {
            INDEX targetIdx = idxToValuesPair.first;
            for (auto &targetVal : idxToValuesPair.second) {
                removeLink(sourceIdx, sourceVal, targetIdx, targetVal);
                if (!hasPointerToIdx(targetIdx, targetVal, sourceIdx)) {
                    toRemove.push_back({targetIdx, targetVal});
                }
            }
        }
        // erase ref
        table[sourceIdx].erase(sourceVal);
    }
}

vector<VALUE> ResultTable::getValues(INDEX refIndex) {
    assertIndex(refIndex);
    vector<VALUE> res;
    for (auto& valueToPointers : table[refIndex]) {
        res.push_back(valueToPointers.first);
    }
    return res;
}

bool ResultTable::hasLink(INDEX refIndex1, VALUE value1, INDEX refIndex2, VALUE value2) {
    assertIndex(refIndex1);
    assertIndex(refIndex2);
    VALUE_TO_POINTERS_MAP *map = &table[refIndex1];
    if (map->find(value1) == map->end()) {
        return false;
    }
    IDX_TO_VALUES_MAP *pIndexToValue = &(*map)[value1];
    if (pIndexToValue->find(refIndex2) == pIndexToValue->end()) {
        return false;
    }
    VALUE_SET *pValues = &(*pIndexToValue)[refIndex2];
    return pValues->find(value2) != pValues->end();
}

void ResultTable::removeLink(INDEX refIndex1, VALUE value1, INDEX refIndex2, VALUE value2) {
    if (!hasLink(refIndex1, value1, refIndex2, value2)) {
        return;
    }

    table[refIndex1][value1][refIndex2].erase(value2);
    if (table[refIndex1][value1][refIndex2].size() == 0) {
        table[refIndex1][value1].erase(refIndex2);
    }

    table[refIndex2][value2][refIndex1].erase(value1);
    if (table[refIndex2][value2][refIndex1].size() == 0) {
        table[refIndex2][value2].erase(refIndex1);
    }
}

void ResultTable::assertIndex(INDEX idx) {
    if (idx < 0 || idx >= table.size()) {
        throw runtime_error("Index out of bound");
    }
}
