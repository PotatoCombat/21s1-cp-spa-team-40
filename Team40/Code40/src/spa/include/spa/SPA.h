#pragma once

#include <string>
#include <list>

#include "pkb/PKB.h"
#include "query_processor/QueryProcessor.h"
#include "source_processor/SourceProcessor.h"

class SPA {
public:
    SPA();

    virtual void processSource(string filename);
    virtual void processQuery(string query, list<string> &results);

private:
    PKB pkb;
    SourceProcessor sourceProcessor = SourceProcessor(nullptr);
    QueryProcessor queryProcessor = QueryProcessor(nullptr);
};
