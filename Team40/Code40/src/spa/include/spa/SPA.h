#pragma once
//#include "pkb/PKB.h"
#include "source_processor/Parser.h"
#include "source_processor/ProgramParser.h"
#include <string>

class SPA {
private:
    // PKB pkb;
    Parser parser;
    ProgramParser programParser;

public:
    void processSource(string filename);
    // void processQuery(string query, list<string> &results);
};
