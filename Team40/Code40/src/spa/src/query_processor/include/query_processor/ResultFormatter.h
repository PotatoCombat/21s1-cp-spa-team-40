#pragma once

#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ResultFormatter {
public:
    static vector<string> formatResult(vector<vector<string>> unformattedRes);
};
