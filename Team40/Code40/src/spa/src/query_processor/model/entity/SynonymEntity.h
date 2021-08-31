#pragma once

#include "Entity.h"

class SynonymEntity : public Entity {
public:
	bool isSynonymEntity() {
		return true;
	}
};