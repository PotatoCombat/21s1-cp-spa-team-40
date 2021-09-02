#pragma once

#include '../relation/Relation.h'
#include '../EntityReference.h'
#include 'SuchThatClause.h'
#include 'PatternClause.h'

using namespace std;

class SelectClause {
private:
    // vector<Declaration> declarations
    vector<SuchThatClause> stClauses;
    vector<PatternClause> ptClauses;

public:
    SelectClause() = 0;
    SelectClause(vector<SuchThatClause> stClauses,
                 vector<PatternClause> ptClauses)
        : stClauses(stClauses), ptClauses(ptClauses) {}

    vector<SuchThatClause> getSuchThatClauses() { return stClauses; }
    vector<PatternClause> getPatternClauses() { return ptClauses; }

    void setSuchThatClause(SuchThatClause stClause) {
        stClauses.push_back(stClause);
    }
    void setPatternClause(PatternClause ptClause) {
        ptClauses.push_back(ptClause);
    }
};
