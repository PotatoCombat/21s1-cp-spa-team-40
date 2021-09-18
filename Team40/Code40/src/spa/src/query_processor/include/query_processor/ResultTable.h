#pragma once

#include <string>
#include <utility>
#include <set>
#include <vector>

#include "query_processor/ValueToPointersMap.h"

using namespace std;

class ResultTable {
private:
    vector<vector<ValueToPointersMap>> mapTable;

public:
    void clear();

    void init(int i);

    int findMapIndex(int refIndex, string value);

    void addValue(int refIndex, string value, set<pair<int, string>> pointers);

    void addValue(int refIndex, vector<ValueToPointersMap> maps);

    set<pair<int, string>> getPointers(int refIndex, string value);

    void removePointer(int refIndex, string value, pair<int, string> pointer);

    bool hasPointerToRef(int sourceRefIndex, string sourceValue,
                         int targetRefIndex);

    void removeMap(int refIndex, string value);

    vector<string> getValues(int refIndex);
};
