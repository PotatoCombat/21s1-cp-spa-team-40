#include "Result.h"

Result::Result() {
	isValid = true;
	hasResult1 = false;
	hasResult2 = false;
	entity1 = NULL;
	entity2 = NULL;
}

void Result::setValid(bool isValid) {
	this->isValid = isValid;
}


void Result::setResultList1(Entity* entity, vector<string> resultList) {
	entity1 = entity;
	resultList1 = resultList;
}

void Result::setResultList2(Entity* entity, vector<string> resultList) {
	entity2 = entity;
	resultList2 = resultList;
}

bool Result::isResultValid() {
	return isValid;
}

Entity* Result::getEntity1() {
	return entity1;
}

Entity* Result::getEntity2() {
	return entity2;
}

bool Result::hasResultList1() { return getEntity1() != NULL; }

bool Result::hasResultList2() { return getEntity2() != NULL; }

vector<string> Result::getResultList1() {
	return resultList1;
}

vector<string> Result::getResultList2() {
	return resultList2;
}