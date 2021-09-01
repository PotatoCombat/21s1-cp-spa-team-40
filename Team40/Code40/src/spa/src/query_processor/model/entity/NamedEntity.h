#pragma once

#include <string>

#include "Entity.h"

using namespace std;

class NamedEntity : public Entity {
public:
	NamedEntity(string name) : Entity(name) {}
	bool IsSynonymEntity() {
		return false;
	}
};