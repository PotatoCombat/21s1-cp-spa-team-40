#include "Entity.h"

const string Entity::WILDCARD = "_";

Entity::Entity(string name) { this->name = name; }

string Entity::getName() { return name; }

EntityType Entity::getType() { return EntityType::UNKNOWN; }

bool Entity::isSynonymEntity() { return true; }

bool Entity::equals(Entity &other) { 
	return this->getName() == other.getName() &&
		this->getType() == other.getType() &&
		this->isSynonymEntity() == other.isSynonymEntity();
}
