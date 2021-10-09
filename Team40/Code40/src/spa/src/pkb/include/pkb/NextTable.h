#pragma once

#include "common/model/Abstractions.h"
#include "common/model/Statement.h"
#include <map>

class NextTable {

public:
    NextTable();

    /// Stores Next(stmt1, stmt2).
    void insertNext(Statement *stmt1, Statement *stmt2);

    /// Selects s such that Next(line, s).
    set<ProgLineIndex> getNextLines(ProgLineIndex line);

    /// Selects s such that Next(s, line).
    set<ProgLineIndex> getPreviousLines(ProgLineIndex line);

    /// Selects BOOLEAN such that Next(line1, line2).
    bool next(ProgLineIndex line1, ProgLineIndex line2);

private:
    /// Stores s such that Follows(stmt, s).
    map<ProgLineIndex, set<ProgLineIndex>> childMap;

    /// Stores s such that Follows(s, stmt).
    map<ProgLineIndex, set<ProgLineIndex>> parentMap;
};
