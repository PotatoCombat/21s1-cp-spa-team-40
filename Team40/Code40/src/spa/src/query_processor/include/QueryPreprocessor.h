#pragma once

#include "model/Query.h"

using namespace std;

class QueryPreprocessor {
public:
    QueryPreprocessor();
    Query preprocessQuery(const string input);
};
