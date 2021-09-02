#include "PatternClause.h"

PatternClause::PatternClause(string syn, EntityReference entRef)
    : syn(syn), entRef(entRef) {}

string PatternClause::getSynonym() { return syn; }
EntityReference PatternClause::getEntRef() { return entRef; }

void PatternClause::setSynonym(string syn) { this->syn = syn; }
void PatternClause::setEntRef(EntityReference entRef) { this->entRef = entRef; }
