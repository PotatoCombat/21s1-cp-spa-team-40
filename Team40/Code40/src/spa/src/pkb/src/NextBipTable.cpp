#include "pkb/NextBipTable.h"

NextBipTable::NextBipTable() = default;

void NextBipTable::insertNextBip(Statement *stmt1, Statement *stmt2) {
    ProgLineIndex line1 = stmt1->getIndex();
    ProgLineIndex line2 = stmt2->getIndex();

    auto searchForChildSet = childMap.find(line1);
    if (searchForChildSet == childMap.end()) {
        childMap[line1] = {line2};
    } else {
        searchForChildSet->second.insert(line2);
    }

    auto searchForParentSet = parentMap.find(line2);
    if (searchForParentSet == parentMap.end()) {
        parentMap[line2] = {line1};
    } else {
        searchForParentSet->second.insert(line1);
    }
}

set<ProgLineIndex> NextBipTable::getNextBipLines(ProgLineIndex line) {
    auto searchForChildSet = childMap.find(line);
    if (searchForChildSet == childMap.end()) {
        return {};
    }
    return searchForChildSet->second;
}

set<ProgLineIndex> NextBipTable::getPreviousBipLines(ProgLineIndex line) {
    auto searchForParentSet = parentMap.find(line);
    if (searchForParentSet == parentMap.end()) {
        return {};
    }
    return searchForParentSet->second;
}

bool NextBipTable::nextBip(ProgLineIndex line1, ProgLineIndex line2) {
    auto searchForChildSet = childMap.find(line1);
    if (searchForChildSet == childMap.end()) {
        return false;
    }
    set<ProgLineIndex> nextBipLines = searchForChildSet->second;
    return nextBipLines.find(line2) != nextBipLines.end();
}
