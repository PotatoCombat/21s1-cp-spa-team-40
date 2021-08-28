#pragma once

#include <Entity.h>

class Relationship {
private:
	Entity firstEntity;
	Entity secondEntity;
public:
	Relationship(Entity firstEntity, Entity secondEntity);
	Entity getFirstEntity();
	Entiry getSecondEntity();
};