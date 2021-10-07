#pragma once

#include <string>
#include <sstream>
#include <vector>

using namespace std;

class ResultFormatter {
public:
    static vector<string> formatResult(vector<vector<string>> unformattedRes);
};
