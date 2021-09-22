# pragma once

#include <vector>

#include "query_processor/model/Reference.h"
#include "query_processor/ValueToPointersMap.h"

using namespace std;

class Result {
private:
	bool isValid; // false if a True/False returns false, true otherwise
	Reference* reference1;
	Reference* reference2;
	vector<ValueToPointersMap> resultList1;
	vector<ValueToPointersMap> resultList2;
public:
	Result() : isValid(true), reference1(NULL), reference2(NULL) {}
	void setValid(bool isValid);
	bool isResultValid();
	void setResultList1(Reference* reference, vector<ValueToPointersMap> resultList);
	void setResultList2(Reference* reference, vector<ValueToPointersMap> resultList);
	bool hasResultList1();
	bool hasResultList2();
	vector<ValueToPointersMap> getResultList1();
	vector<ValueToPointersMap> getResultList2();
	Reference* getReference1();
	Reference* getReference2();
	bool equals(Result &other);
};
