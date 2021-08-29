using namespace std;

#include "Entity.h"

Entity::Entity(string name) {
	this->name = name;
}

string Entity::getName() {
	return name;
}

EntityType Entity::getType() {
	return EntityType::ENTITY;
}
