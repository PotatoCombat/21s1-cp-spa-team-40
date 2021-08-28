#pragma once

#include <string>
#include <vector>

#include "Entity.h"
#include "Relationship.h"

using namespace std;

// typedef vector<Entity> LIST_OF_ENTITY;

class Query {
private:
	vector<Entity> entityList;
	vector<Relationship> relationshipList;
	Entity returnEntity;
public:
	Query();

	void setReturnEntity(Entity entity);
	Entity getReturnEntity();

	void addEntity(Entity entity);
	vector<Entity> getEntities();

	void addRelationship(Relationship relationship);
	vector<Relationship> getRelationships();

}