#include "query_processor/ResultTable.h"

void ResultTable::clear() { table.clear(); }

void ResultTable::init(int i) {
    vector<VALUE_TO_POINTERS_MAP> temp(i, VALUE_TO_POINTERS_MAP{});
    table = temp;
}

void ResultTable::addValue(int refIndex, string value, POINTER_SET pointers) {
    if (refIndex < 0 || refIndex >= table.size()) {
        throw runtime_error("Index out of bound");
    }
    VALUE_TO_POINTERS_MAP *map = &table[refIndex];
    if (map->find(value) == map->end()) {
        (*map)[value] = pointers;
    } else {
        (*map)[value].insert(pointers.begin(), pointers.end());
    }
}

void ResultTable::addValues(int refIndex, VALUE_TO_POINTERS_MAP map) {
    for (auto valueToPointers : map) {
        addValue(refIndex, valueToPointers.first, valueToPointers.second);
    }
}

POINTER_SET ResultTable::getPointers(int refIndex, string value) {
    if (refIndex < 0 || refIndex >= table.size()) {
        throw runtime_error("Index out of bound");
    }
    VALUE_TO_POINTERS_MAP *map = &table[refIndex];
    if (map->find(value) == map->end()) {
        return POINTER_SET{};
    } else {
        return (*map)[value];
    }
}
void ResultTable::removePointer(int refIndex, string value, POINTER pointer) {
    if (refIndex < 0 || refIndex >= table.size()) {
        throw runtime_error("Index out of bound");
    }
    VALUE_TO_POINTERS_MAP *map = &table[refIndex];
    if (map->find(value) != map->end()) {
        (*map)[value].erase(pointer);
    }
}
bool ResultTable::hasPointerToRef(int sourceRefIndex, string sourceValue,
                     int targetRefIndex) {
    if (sourceRefIndex < 0 || sourceRefIndex >= table.size()) {
        throw runtime_error("Index out of bound");
    }
    if (targetRefIndex < 0 || targetRefIndex >= table.size()) {
        throw runtime_error("Index out of bound");
    }

    VALUE_TO_POINTERS_MAP *map = &table[sourceRefIndex];
    if (map->find(sourceValue) == map->end()) {
        return false;
    }

    for (POINTER pointer : (*map)[sourceValue]) {
        if (pointer.first == targetRefIndex) {
            return true;
        }
    }

    return false;
}

void ResultTable::removeMap(int refIndex, string value) {
    set<POINTER> visited;
    vector<POINTER> toRemove{make_pair(refIndex, value)};
    while (!toRemove.empty()) {
        POINTER ref = toRemove.back();
        toRemove.pop_back();
        if (visited.find(ref) != visited.end()) {
            continue;
        }
        visited.insert(ref);
        int sourceIndex = ref.first;
        string sourceValue = ref.second;
        POINTER_SET refPointers = getPointers(sourceIndex, sourceValue);
        // remove pointers from other values to ref
        for (auto refPointer : refPointers) {
            int targetIndex = refPointer.first;
            string targetValue = refPointer.second;

            removePointer(targetIndex, targetValue, ref);
            if (!hasPointerToRef(refPointer.first, refPointer.second,
                                 sourceIndex)) {
                toRemove.push_back(refPointer);
            }
        }
        // erase ref
        table[sourceIndex].erase(sourceValue);
    }
}

vector<string> ResultTable::getValues(int refIndex) {
    vector<string> res;
    for (auto valueToPointers : table[refIndex]) {
        res.push_back(valueToPointers.first);
    }
    return res;
}

bool ResultTable::hasLink(int refIndex1, string value1, int refIndex2, string value2) {
    VALUE_TO_POINTERS_MAP *map = &table[refIndex1];
    if (map->find(value1) == map->end()) {
        return false;
    }
    return (*map)[value1].find({refIndex2, value2}) != (*map)[value1].end();
}

void ResultTable::removeLink(int refIndex1, string value1, int refIndex2, string value2) {
    if (!hasLink(refIndex1, value1, refIndex2, value2)) {
        return;
    }
    table[refIndex1][value1].erase({refIndex2, value2});
    table[refIndex2][value2].erase({refIndex1, value1});
}
