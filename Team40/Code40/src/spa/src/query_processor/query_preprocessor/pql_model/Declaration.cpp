#include "Declaration.h"

Declaration::Declaration(string syn, DesignEntityType type) {
    this->syn = syn;
    this->type = type;
}

string Declaration::getSynonym() { return this->syn; }

DesignEntityType Declaration::getType() { return this->type; }
