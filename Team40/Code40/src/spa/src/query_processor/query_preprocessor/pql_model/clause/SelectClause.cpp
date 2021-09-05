#include "SelectClause.h"

SelectClause::SelectClause() = default;

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

void SelectClause::addDeclaration(Declaration declaration) {
    declarations.push_back(declaration);
}
void SelectClause::addSuchThatClause(SuchThatClause stClause) {
    stClauses.push_back(stClause);
}
void SelectClause::addPatternClause(PatternClause ptClause) {
    ptClauses.push_back(ptClause);
}
