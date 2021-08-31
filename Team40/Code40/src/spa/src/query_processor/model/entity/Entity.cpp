using namespace std;

#include "Entity.h"

Entity::Entity(string name) { this->name = name; }

string Entity::GetName() { return name; }

bool Entity::operator==(const Entity &other) { 
	return this->getType() == other.getType() &&
           this->getName() == other.getName() &&
           this->isSynonymEntity() == other.isSynonymEntity();
}
