#include "query_processor/ValueToPointersMap.h"

bool ValueToPointersMap::isMapOfValue(string value) { return this->value == value; }

ValueToPointersMap::ValueToPointersMap(string value,
                                     set<pair<int, string>> pointers) {
    this->value = value;
    this->pointers = pointers;
}
void ValueToPointersMap::insert(pair<int, string> pointer) {
    pointers.insert(pointer);
}
void ValueToPointersMap::insert(set<pair<int, string>> pointers) {
    for (auto pointer : pointers) {
        this->insert(pointer);
    }
}
set<pair<int, string>> ValueToPointersMap::getPointers() { return pointers; }

void ValueToPointersMap::erasePointer(pair<int, string> pointer) {
    pointers.erase(pointer);
}

bool ValueToPointersMap::hasPointerToRef(int refIndex) {
    for (auto pointer : pointers) {
        if (pointer.first == refIndex) {
            return true;
        }
    }
    return false;
}

bool ValueToPointersMap::hasLink(int refIndex, string value) {
    return pointers.find(make_pair(refIndex, value)) != pointers.end();
}

string ValueToPointersMap::getValue() { return value; }

bool ValueToPointersMap::equals(ValueToPointersMap& rhs) {
    return this->getValue() == rhs.getValue() &&
           this->getPointers() == rhs.getPointers();
}
