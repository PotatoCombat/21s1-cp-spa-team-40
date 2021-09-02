#include "Declaration.h"

Declaration::Declaration(string syn) { this->syn = syn; }

string Declaration::getSynonym() { return this->syn; }

DesignEntityType Declaration::getType() { return DesignEntityType::UNKNOWN; }
