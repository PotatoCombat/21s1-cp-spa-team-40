#include "query_processor/ResultFormatter.h"

vector<string> ResultFormatter::formatResult(vector<vector<string>> unformattedRes) {
    vector<string> result;
    for (vector<string> r : unformattedRes) {
        ostringstream ss;
        copy(r.begin(), r.end() - 1, ostream_iterator<string>(ss, " "));
        ss << r.back();
        result.push_back(ss.str());
    }
    return result;
}