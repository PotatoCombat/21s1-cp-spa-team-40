#pragma once

#include <map>
#include <string>
#include <set>
#include <utility>
#include <vector>
#include <stdexcept>

using namespace std;

typedef pair<int, string> POINTER;
typedef set<POINTER> POINTER_SET;
typedef map<string, POINTER_SET> VALUE_TO_POINTERS_MAP;

class ResultTable {
private:
    vector<VALUE_TO_POINTERS_MAP> table;

public:
    void clear();

    void init(int i);

    void addValue(int refIndex, string value, POINTER_SET pointers);

    void addValues(int refIndex, VALUE_TO_POINTERS_MAP map);

    POINTER_SET getPointers(int refIndex, string value);

    void removePointer(int refIndex, string value, POINTER pointer);

    bool hasPointerToRef(int sourceRefIndex, string sourceValue,
                         int targetRefIndex);

    void removeMap(int refIndex, string value);

    vector<string> getValues(int refIndex);

    bool hasLink(int refIndex1, string value1, int refIndex2, string value2);

    void removeLink(int refIndex1, string value1, int refIndex2, string value2);
};
