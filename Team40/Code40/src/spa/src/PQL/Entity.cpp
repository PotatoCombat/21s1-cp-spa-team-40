using namespace std;

#include "Entity.h"

Entity::Entity(string n, EntityType t) {
	name = n;
	type = t;
}

EntityType Entity::getType() {
	return type;
}

string Entity::getName() {
	return name;
}