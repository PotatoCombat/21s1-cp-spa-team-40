#pragma once

#include "../model/Query.h"
#include "QueryTokenizer.h"
#include "QueryParser.h"
#include <string>

using namespace std;

class QueryPreprocessor {
private:
    QueryTokenizer tokenizer;
    QueryParser parser;

public:
    QueryPreprocessor();
    Query preprocessQuery(string input);
};
