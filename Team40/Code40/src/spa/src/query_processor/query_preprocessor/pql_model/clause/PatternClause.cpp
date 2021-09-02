#include "PatternClause.h"

PatternClause::PatternClause(string syn, EntityReference entRef,
                             Expression expr)
    : syn(syn), entRef(entRef), expr(expr) {}

string PatternClause::getSynonym() { return this->syn; }
EntityReference PatternClause::getEntRef() { return this->entRef; }
Expression PatternClause::getExpression() { return this->expr; }

void PatternClause::setSynonym(string syn) { this->syn = syn; }
void PatternClause::setEntRef(EntityReference entRef) { this->entRef = entRef; }
void PatternClause::setExpression(Expression expr) { this->expr = expr; }
