#include "source_processor/parser/EntityParser.h"
#include <algorithm>

EntityParser::EntityParser(vector<string> content, int index) : content(content), index(index){};
bool EntityParser::isValidName(string input) {
    // NAME: LETTER (LETTER | DIGIT)*
    // procedure names and variables are strings of letters, and digits,
    // starting with a letter
    if (!isalpha(input.at(0))) {
        return false;
    }
    return find_if(input.begin(), input.end(), [](char c) { return !(isalnum(c)); }) == input.end();
}