#include "query_processor/ResultTable.h"

void ResultTable::clear() { table.clear(); }

void ResultTable::init(int size) {
    vector<VALUE_TO_POINTERS_MAP> temp(size, VALUE_TO_POINTERS_MAP{});
    table = temp;
}

void ResultTable::addValue(INDEX idx, VALUE val) {
    assertIndex(idx);
    VALUE_TO_POINTERS_MAP *map = &table[idx];
    if (map->find(val) == map->end()) {
        (*map)[val] = IDX_TO_VALUES_MAP{};
    }
}

void ResultTable::addValueWithLink(INDEX sourceIdx, VALUE sourceVal, INDEX targetIdx, VALUE_SET targetVals) {
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

vector<INDEX> ResultTable::getLinkedIndexes(INDEX idx) {
    vector<INDEX> res;
    assertIndex(idx);
    VALUE_TO_POINTERS_MAP vtp = table[idx];
    if (vtp.empty()) {
        return res;
    }

    IDX_TO_VALUES_MAP itv = vtp.begin()->second;
    for (auto idxValsPair : itv) {
        res.push_back(idxValsPair.first);
    }
    return res;
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
    if (pIndexToValue->find(targetIdx) == pIndexToValue->end()) {
        return false;
    }

    return pIndexToValue[targetIdx].size() > 0;
}

VALUE_SET ResultTable::getPointersToIdx(INDEX sourceIdx, VALUE sourceValue,
                                        INDEX targetIdx) {
    VALUE_TO_POINTERS_MAP *map = &table[sourceIdx];
    if (map->find(sourceValue) == map->end()) {
        return VALUE_SET{};
    }

    IDX_TO_VALUES_MAP *pIndexToValue = &(*map)[sourceValue];
    if (pIndexToValue->find(targetIdx) == pIndexToValue->end()) {
        return VALUE_SET{};
    }

    return (*pIndexToValue)[targetIdx];
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
            for (string targetVal : idxToValuesPair.second) {
                table[targetIdx][targetVal][sourceIdx].erase(sourceVal);
                if (table[targetIdx][targetVal][sourceIdx].size() == 0) {
                    table[targetIdx][targetVal].erase(sourceIdx);
                }
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

bool ResultTable::isColumnEmpty(INDEX refIndex) {
    assertIndex(refIndex);
    return table[refIndex].size() == 0;
}

bool ResultTable::hasLinkBetweenValues(INDEX refIndex1, VALUE value1, INDEX refIndex2, VALUE value2) {
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
    if (!hasLinkBetweenValues(refIndex1, value1, refIndex2, value2)) {
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

bool ResultTable::hasVal(INDEX idx, VALUE val) {
    assertIndex(idx);
    return table[idx].find(val) != table[idx].end();
}

vector<vector<string>> ResultTable::generateResult(vector<INDEX> indexes) {
    string EMPTY = "";
    set<INDEX> visited;
    // the inner vector has element equal to the number of indexes in this table
    vector<vector<string>> combinations{vector<string>(table.size(), EMPTY)};
    
    // evaluate each groups
    for (INDEX idx : indexes) {
        // 1st idx is the idx to be eval, 
        // 2nd idx is the index that calls this index

        vector<INDEX> toBeEval;
        set<INDEX> visitedInGroup; // so that don't need to check all visited idx
        toBeEval.push_back(idx);
        while (!toBeEval.empty()) {
            INDEX currIdx = toBeEval.back();
            toBeEval.pop_back();
            vector<vector<string>> newRes;
            if (visited.find(currIdx) != visited.end()) { // if visited already then continue
                continue;
            }

            for (vector<string> v : combinations) {
                bool linkAdded = false;
                for (string value : getValues(currIdx)) {
                    bool valid = true;
                    for (INDEX vIdx : visitedInGroup) {
                        if (hasPointerToIdx(currIdx, value, vIdx) &&
                            !hasLinkBetweenValues(currIdx, value, vIdx, v[vIdx])) {
                            valid = false;
                            break;
                        }
                    }

                    // if has link to values of all visited indexes
                    if (valid) {
                        vector<string> temp = v;
                        temp[currIdx] = value;
                        newRes.push_back(temp);
                    }
                }
            }

            for (auto otherIndex : getLinkedIndexes(currIdx)) {
                toBeEval.push_back(otherIndex);
            }

            combinations = newRes;
            visited.insert(currIdx);
            visitedInGroup.insert(currIdx);
        }
    }

    // generate result
    vector<vector<string>> finalRes;
    set<vector<string>> generatedAlready;
    for (vector<string> v : combinations) {
        vector<string> res;
        for (INDEX i : indexes) {
            res.push_back(v[i]);
        }
        if (generatedAlready.count(res) == 0) {
            finalRes.push_back(res);
            generatedAlready.insert(res);
        }
    }

    return finalRes;
}

void ResultTable::assertIndex(INDEX idx) {
    if (idx < 0 || idx >= table.size()) {
        throw runtime_error("Index out of bound");
    }
}
