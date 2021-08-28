#include "Query.h"

using namespace std;

Query::Query() {
	vector<Entity> entityList;
	vector<Relationship> relationshipList;
	Entity returnEntity;
}

Entity Query::getReturnEntity() {
	return returnEntity;
}

void Query::setReturnEntity(Entity entity) {
	returnEntity = entity;
}

void Query::addEntity(Entity entity) {
	entityList.push_back(entity);
}

vector<Entity> Query::getEntities() {
	return entityList;
}

void Query::addRelationship(Relationship relationship) {
	relationshipList.push_back(relationship);
}

vector<Relationship> Query::getRelationships() {
	return relationshipList;
}
