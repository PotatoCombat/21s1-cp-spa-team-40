using namespace std;

#include "Entity.h"

Entity::Entity(string name) { this->name = name; }

string Entity::GetName() { return name; }

EntityType Entity::GetType() { return EntityType::UNKNOWN; }

bool Entity::operator==(const Entity &other) { return name == other.name; }
