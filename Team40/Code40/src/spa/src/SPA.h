#pragma once
#include "PKB.h"
#include "source_processor/include/Parser.h"
#include <list>
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
