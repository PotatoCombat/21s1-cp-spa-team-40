#pragma once

#include "Entity.h"

class SynonymEntity : public Entity {
public:
    SynonymEntity(string name) : Entity(name) {}
	bool isSynonymEntity() {
		return true;
	}
};
