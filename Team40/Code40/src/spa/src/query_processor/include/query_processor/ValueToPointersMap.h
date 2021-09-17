#pragma once

#include <string>
#include <utility>
#include <set>

using namespace std;

class ValueToPointersMap {
private:
    string value;
    set<pair<int, string>> pointers;

public:
    bool isMapOfValue(string value);

    ValueToPointersMap(string value, set<pair<int, string>> pointers);

    void insert(pair<int, string> pointer);

    void insert(set<pair<int, string>> pointers);

    set<pair<int, string>> getPointers();

    void erasePointer(pair<int, string> pointer);

    bool hasPointerToRef(int refIndex);

    string getValue();
};
