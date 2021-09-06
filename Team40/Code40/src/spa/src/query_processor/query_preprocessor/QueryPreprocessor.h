#pragma once

#include "../model/Query.h"
#include <string>

using namespace std;

class QueryPreprocessor {
public:
    QueryPreprocessor();
    Query preprocessQuery(string input);
};
