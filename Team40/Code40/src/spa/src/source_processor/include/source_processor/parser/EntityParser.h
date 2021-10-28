#pragma once
#include "common/model/Abstractions.h"
#include "common/model/Entity.h"
#include <string>
#include <vector>

using namespace std;
template <typename T> class EntityParser {
protected:
    vector<string> content;
    int index;
    static int dummyProgramIndex;
    // T t;
    // Entity<class T> entity;
    // Statement *stmt;
    // Procedure *proc;
    explicit EntityParser(vector<string> content, int index) : content(content), index(index){};
    bool isValidName(string input) {
        // NAME: LETTER (LETTER | DIGIT)*
        // procedure names and variables are strings of letters, and digits,
        // starting with a letter
        if (!isalpha(input.at(0))) {
            return false;
        }
        return find_if(input.begin(), input.end(), [](char c) { return !(isalnum(c)); }) ==
               input.end();
    }

public:
    T *parseEntity(int &programIndex = dummyProgramIndex);
    // virtual Procedure *parseProcedure();
    // virtual Statement *parseStatement();
    // virtual Statement *parseParentStatement(optional<int> &programIndex);
    // virtual void parseChildStatements(optional<int> &programIndex);
};
