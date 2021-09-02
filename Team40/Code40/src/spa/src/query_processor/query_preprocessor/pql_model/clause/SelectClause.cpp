#include "SelectClause.h"

SelectClause::SelectClause(vector<SuchThatClause> stClauses,
                           vector<PatternClause> ptClauses)
    : stClauses(stClauses), ptClauses(ptClauses) {}

vector<SuchThatClause> SelectClause::getSuchThatClauses() { return stClauses; }
vector<PatternClause> SelectClause::getPatternClauses() { return ptClauses; }

void SelectClause::setSuchThatClause(SuchThatClause stClause) {
    stClauses.push_back(stClause);
}
void SelectClause::setPatternClause(PatternClause ptClause) {
    ptClauses.push_back(ptClause);
}
