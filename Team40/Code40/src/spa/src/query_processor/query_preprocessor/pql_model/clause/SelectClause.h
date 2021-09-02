#pragma once

#include "../Declaration.h"
#include "../EntityReference.h"
#include "../relation/Relation.h"
#include "PatternClause.h"
#include "SuchThatClause.h"
#include <string>
#include <vector>

using namespace std;

class SelectClause {
private:
    string returnEntity; // essentially synonym
    vector<Declaration> declarations;
    vector<SuchThatClause> stClauses;
    vector<PatternClause> ptClauses;

public:
    SelectClause();
    SelectClause(string returnEntity, vector<Declaration> declarations,
                 vector<SuchThatClause> stClauses,
                 vector<PatternClause> ptClauses);

    string getReturnEntity();
    vector<Declaration> getDeclarations();
    vector<SuchThatClause> getSuchThatClauses();
    vector<PatternClause> getPatternClauses();

    void setReturnEntity(string returnEntity);
    void setDeclaration(Declaration declaration);
    void setSuchThatClause(SuchThatClause stClause);
    void setPatternClause(PatternClause ptClause);
};
