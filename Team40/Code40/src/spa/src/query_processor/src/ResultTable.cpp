#include "query_processor/ResultTable.h"

void ResultTable::clear() { mapTable.clear(); }

void ResultTable::init(int i) {
    vector<vector<ValueToPointersMap>> temp(i, vector<ValueToPointersMap>{});
    mapTable = temp;
}

int ResultTable::findMapIndex(int refIndex, string value) {
    vector<ValueToPointersMap> mapList = mapTable[refIndex];
    for (int i = 0; i < mapList.size(); i++) {
        if (mapList[i].isMapOfValue(value)) {
            return i;
        }
    }
    return -1;
}

void ResultTable::addValue(int refIndex, string value, set<pair<int, string>> pointers) {
    int mapIndex = findMapIndex(refIndex, value);
    if (mapIndex == -1) {
        ValueToPointersMap map(value, pointers);
        mapTable[refIndex].push_back(map);
    } else {
        mapTable[refIndex][mapIndex].insert(pointers);
    }
}

void ResultTable::addValue(int refIndex, vector<ValueToPointersMap> maps) {
    for (auto map : maps) {
        addValue(refIndex, map.getValue(), map.getPointers());
    }
}

set<pair<int, string>> ResultTable::getPointers(int refIndex, string value) {
    int mapIndex = findMapIndex(refIndex, value);
    if (mapIndex == -1) {
        return set<pair<int, string>>{};
    } else {
        return mapTable[refIndex][mapIndex].getPointers();
    }
}
void ResultTable::removePointer(int refIndex, string value, pair<int, string> pointer) {
    int mapIndex = findMapIndex(refIndex, value);
    if (mapIndex != -1) {
        mapTable[refIndex][mapIndex].erasePointer(pointer);
    }
}
bool ResultTable::hasPointerToRef(int sourceRefIndex, string sourceValue,
                     int targetRefIndex) {
    int mapIndex = findMapIndex(sourceRefIndex, sourceValue);
    if (mapIndex != -1) {
        return mapTable[sourceRefIndex][mapIndex]
            .hasPointerToRef(targetRefIndex);
    }
    return false;
}

void ResultTable::removeMap(int refIndex, string value) {
    set<pair<int, string>> visited;
    vector<pair<int, string>> toRemove{make_pair(refIndex, value)};
    while (!toRemove.empty()) {
        pair<int, string> ref = toRemove.back();
        toRemove.pop_back();
        if (visited.find(ref) != visited.end()) {
            continue;
        }
        visited.insert(ref);
        set<pair<int, string>> refPointers = getPointers(ref.first, ref.second);
        // remove pointers to ref
        for (auto refPointer : refPointers) {
            removePointer(refPointer.first, refPointer.second, ref);
            if (!hasPointerToRef(refPointer.first, refPointer.second,
                                 ref.first)) {
                toRemove.push_back(refPointer);
            }
        }
        int mapIndex = findMapIndex(ref.first, ref.second);
        if (mapIndex != -1) {
            mapTable[ref.first].erase(mapTable[ref.first].begin() + mapIndex);
        }
    }
}

vector<string> ResultTable::getValues(int refIndex) {
    vector<string> res;
    for (auto valueToPointerMap : mapTable[refIndex]) {
        res.push_back(valueToPointerMap.getValue());
    }
    return res;
}

bool ResultTable::hasLink(int refIndex1, string value1, int refIndex2, string value2) {
    int i = findMapIndex(refIndex1, value1);
    if (i == -1) {
        return false;
    }
    return mapTable[refIndex1][i].hasLink(refIndex2, value2);
}

void ResultTable::removeLink(int refIndex1, string value1, int refIndex2, string value2) {
    if (!hasLink(refIndex1, value1, refIndex2, value2)) {
        return;
    }
    int i = findMapIndex(refIndex1, value1);
    mapTable[refIndex1][i].erasePointer(make_pair(refIndex2, value2));
    int i2 = findMapIndex(refIndex2, value2);
    mapTable[refIndex2][i].erasePointer(make_pair(refIndex1, value1));
}
