#pragma once

#include "Entity.h"

class NamedEntity : public Entity {
public:
	using Entity::Entity;
	bool isSynonymEntity() {
		return false;
	}
};