#pragma once
#include "PKB.h"
#include "SourceProcessor/Parser.cpp"
#include "SourceProcessor/Parser.h"
#include <list>
#include <string>

using namespace std;

class SPA {
private:
    // PKB pkb;
    Parser parser;

public:
    void processSource(string filename);
    void processQuery(string query, list<string> &results);
};
