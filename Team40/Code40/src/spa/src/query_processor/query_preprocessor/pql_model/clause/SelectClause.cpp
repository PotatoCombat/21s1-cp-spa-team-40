#include "SelectClause.h"

SelectClause::SelectClause(string returnEntity,
                           vector<Declaration> declarations,
                           vector<SuchThatClause> stClauses,
                           vector<PatternClause> ptClauses)
    : returnEntity(returnEntity), declarations(declarations),
      stClauses(stClauses), ptClauses(ptClauses) {}

string SelectClause::getReturnEntity() { return this->returnEntity; }

vector<Declaration> SelectClause::getDeclarations() {
    return this->declarations;
}
vector<SuchThatClause> SelectClause::getSuchThatClauses() {
    return this->stClauses;
}
vector<PatternClause> SelectClause::getPatternClauses() {
    return this->ptClauses;
}

void SelectClause::setReturnEntity(string returnEntity) {
    this->returnEntity = returnEntity;
}

void SelectClause::setDeclaration(Declaration declaration) {
    declarations.push_back(declaration);
}
void SelectClause::setSuchThatClause(SuchThatClause stClause) {
    stClauses.push_back(stClause);
}
void SelectClause::setPatternClause(PatternClause ptClause) {
    ptClauses.push_back(ptClause);
}
