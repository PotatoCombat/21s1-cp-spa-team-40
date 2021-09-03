#include "SuchThatClause.h"

SuchThatClause::SuchThatClause(Relation rel) : rel(rel) {}

Relation SuchThatClause::getRelation() { return rel; }

ClauseType SuchThatClause::getType() { return ClauseType::SUCH_THAT; }

void SuchThatClause::setRelation(Relation rel) { this->rel = rel; }
