#include "pkb/NextTable.h"

NextTable::NextTable() = default;

void NextTable::insertNext(Statement *stmt1, Statement *stmt2) {
    ProgLineIndex line1 = stmt1->getIndex();
    ProgLineIndex line2 = stmt2->getIndex();

    auto searchForChildSet = childMap.find(line1);
    if (searchForChildSet == childMap.end()) {
        childMap[line1] = { line2 };
    } else {
        searchForChildSet->second.insert(line2);
    }

    auto searchForParentSet = parentMap.find(line2);
    if (searchForParentSet == parentMap.end()) {
        parentMap[line2] = { line1 };
    } else {
        searchForParentSet->second.insert(line1);
    }
}

set<ProgLineIndex> NextTable::getNextLines(ProgLineIndex line) {
    auto searchForChildSet = childMap.find(line);
    if (searchForChildSet == childMap.end()) {
        return { };
    }
    return searchForChildSet->second;
}

set<ProgLineIndex> NextTable::getPreviousLines(ProgLineIndex line) {
    auto searchForParentSet = parentMap.find(line);
    if (searchForParentSet == parentMap.end()) {
        return { };
    }
    return searchForParentSet->second;
}

// Next(line1, line2)
bool NextTable::next(ProgLineIndex line1, ProgLineIndex line2) {
    auto searchForChildSet = childMap.find(line1);
    if (searchForChildSet == childMap.end())
    {
        return false;
    }
    set<ProgLineIndex> nextLines = searchForChildSet->second;
    return nextLines.find(line2) != nextLines.end();
}
