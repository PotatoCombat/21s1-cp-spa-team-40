#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Statement.h"
#include <map>

class NextBipTable {
public:
    NextBipTable();

    /// Stores NextBip(stmt1, stmt2).
    void insertNextBip(Statement *stmt1, Statement *stmt2);

    /// Selects s such that NextBip(line, s).
    set<ProgLineIndex> getNextBipLines(ProgLineIndex line);

    /// Selects s such that NextBip(s, line).
    set<ProgLineIndex> getPreviousBipLines(ProgLineIndex line);

    /// Selects BOOLEAN such that NextBip(line1, line2).
    bool nextBip(ProgLineIndex line1, ProgLineIndex line2);

private:
    /// Stores s such that NextBip(stmt, s).
    map<ProgLineIndex, set<ProgLineIndex>> childMap;

    /// Stores s such that NextBip(s, stmt).
    map<ProgLineIndex, set<ProgLineIndex>> parentMap;
};