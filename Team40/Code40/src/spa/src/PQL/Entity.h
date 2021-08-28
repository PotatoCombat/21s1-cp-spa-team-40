#pragma once

#include "string"

using namespace std;


enum class EntityType { Statement };

class Entity {
private:
	string name;
	EntityType type;

public:
	Entity(string n, EntityType t);
	EntityType getType();
	string getName();
};