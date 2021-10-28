#pragma once
#include "common/model/Abstractions.h"
#include "common/model/Entity.h"
#include "source_processor/parser/Tokens.h"
#include <string>
#include <vector>

using namespace std;
template <typename T> class EntityParser {
protected:
    vector<string> content;
    int index;
    static int dummyProgramIndex;

    explicit EntityParser(vector<string> content, int index)
        : content(content), index(index){};

    bool isValidName(string input) {
        // NAME: LETTER (LETTER | DIGIT)*
        // procedure names and variables are strings of letters, and digits,
        // starting with a letter
        if (!isalpha(input.at(0))) {
            return false;
        }
        return find_if(input.begin(), input.end(),
                       [](char c) { return !(isalnum(c)); }) == input.end();
    }

public:
    virtual T *parseEntity(int &programIndex = dummyProgramIndex) {
        return new T();
    };
};
