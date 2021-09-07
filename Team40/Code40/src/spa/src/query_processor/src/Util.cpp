#include "Util.h"

void toString(vector<int> vectorIn, vector<string> vectorOut) {
    transform(vectorIn.begin(), vectorIn.end(), back_inserter(vectorOut),
              [](int i) { return std::to_string(i); });
}