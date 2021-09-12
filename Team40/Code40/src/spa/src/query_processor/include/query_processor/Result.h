# pragma once

#include <vector>

#include "query_processor/model/Reference.h"

using namespace std;

class Result {
private:
	bool isValid; // false if a True/False returns false, true otherwise
	Reference* reference1;
	Reference* reference2;
	vector<string> resultList1;
	vector<string> resultList2;
public:
	Result() : isValid(true), reference1(NULL), reference2(NULL) {}
	void setValid(bool isValid);
	bool isResultValid();
	void setResultList1(Reference* reference, vector<string> resultList);
	void setResultList2(Reference* reference, vector<string> resultList);
	bool hasResultList1();
	bool hasResultList2();
	vector<string> getResultList1();
	vector<string> getResultList2();
	Reference* getReference1();
	Reference* getReference2();
	bool equals(Result &other);
};
