# pragma once

#include <vector>

#include "model/entity/Entity.h"

using namespace std;

class Result {
private:
	bool isValid; // false if a True/False returns false, true otherwise
    bool hasResult1;
    bool hasResult2;
	Entity* entity1;
	Entity* entity2;
	vector<string> resultList1;
	vector<string> resultList2;
public:
	Result();
	void setValid(bool isValid);
	bool isResultValid();
	void setResultList1(Entity* entity, vector<string> resultList);
	void setResultList2(Entity* entity, vector<string> resultList);
	bool hasResultList1();
	bool hasResultList2();
	vector<string> getResultList1();
	vector<string> getResultList2();
	Entity* getEntity1();
	Entity* getEntity2();
};
