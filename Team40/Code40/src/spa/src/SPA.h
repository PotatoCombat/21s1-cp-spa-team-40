#pragma once
#include "PKB.h"
#include "source_processor/Parser.h"
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
