#pragma once

#include "../EntityReference.h"
#include "../relation/Relation.h"
#include "PatternClause.h"
#include "SuchThatClause.h"
#include <vector>

using namespace std;

class SelectClause {
private:
    // vector<Declaration> declarations
    vector<SuchThatClause> stClauses;
    vector<PatternClause> ptClauses;

public:
    SelectClause();
    SelectClause(vector<SuchThatClause> stClauses,
                 vector<PatternClause> ptClauses);

    vector<SuchThatClause> getSuchThatClauses();
    vector<PatternClause> getPatternClauses();

    void setSuchThatClause(SuchThatClause stClause);
    void setPatternClause(PatternClause ptClause);
};
