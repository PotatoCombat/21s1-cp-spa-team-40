#pragma once
#include "common/model/Abstractions.h"
#include "common/model/Entity.h"
#include <string>
#include <vector>

using namespace std;

class EntityParser {
protected:
    vector<string> content;
    int index;
    static int dummyProgramIndex;
    // Entity<class T> entity;
    // Statement *stmt;
    // Procedure *proc;

public:
    EntityParser(vector<string> content, int index);
    // Entity<StmtIndex> *parseEntity(int &programIndex = dummyProgramIndex);
    // virtual Procedure *parseProcedure();
    // virtual Statement *parseStatement();
    // virtual Statement *parseParentStatement(optional<int> &programIndex);
    // virtual void parseChildStatements(optional<int> &programIndex);
    bool isValidName(string input);
};
