#include "SuchThatClause.h"

SuchThatClause::SuchThatClause(Relation rel) : rel(rel) {}

Relation SuchThatClause::getRelation() { return rel; }

void SuchThatClause::setRelation(Relation rel) { this->rel = rel; }