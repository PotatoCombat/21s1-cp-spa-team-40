#pragma once

#include "string"

using namespace std;


enum class EntityType { STATEMENT, ENTITY };

class Entity {
private:
	string name;

public:
	Entity(string name);
	string getName();

	virtual EntityType getType();
};
