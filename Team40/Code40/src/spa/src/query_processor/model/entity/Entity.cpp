#include "Entity.h"

Entity::Entity(string name) { this->name = name; }

string Entity::GetName() { return name; }

EntityType Entity::GetType() { return EntityType::UNKNOWN; }

bool Entity::IsSynonymEntity() { return true; }

bool Entity::Equals(Entity &other) { 
	return this->GetName() == other.GetName() &&
		this->GetType() == other.GetType() &&
		this->IsSynonymEntity() == other.IsSynonymEntity();
}
