#pragma once
#include "PKB.h"
#include <list>
#include <string>
using namespace std;

class SPA {
private:
    // PKB pkb;

public:
    void processSource(string filename);
    void processQuery(string query, list<string> &results);
};
