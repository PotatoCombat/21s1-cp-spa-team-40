#pragma once

//#include "pkb/PKB.h"
// #include "source_processor/Parser.h"

#include "../../src/source_processor/include/source_processor/Parser.h"
#include "../../src/source_processor/src/Parser.cpp"
#include <string>

using namespace std;

class SPA {
private:
    // PKB pkb;
    Parser parser;

public:
    void processSource(string filename);
    // void processQuery(string query, list<string> &results);
};
