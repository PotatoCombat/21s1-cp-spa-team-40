#pragma once

#include "string"

using namespace std;


enum class EntityType { 
	UNKNOWN, 
	STATEMENT 
};

class Entity {
private:
	string name;

public:
	Entity(string name);
	string getName();
	virtual EntityType getType();
};
