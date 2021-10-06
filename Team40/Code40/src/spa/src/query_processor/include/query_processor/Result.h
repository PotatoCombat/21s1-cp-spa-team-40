# pragma once

#include <vector>

#include "query_processor/model/Reference.h"
#include "query_processor/ResultTable.h"

using namespace std;

class Result {
private:
	bool isValid; // false if a True/False returns false, true otherwise
	Reference* reference1;
	Reference* reference2;
	VALUE_TO_POINTERS_MAP resultList1;
	VALUE_TO_POINTERS_MAP resultList2;
public:
	Result() : isValid(true), reference1(NULL), reference2(NULL) {}
	void setValid(bool isValid);
	bool isResultValid();
	void setResultList1(Reference* reference, VALUE_TO_POINTERS_MAP resultList);
	void setResultList2(Reference* reference, VALUE_TO_POINTERS_MAP resultList);
	bool hasResultList1();
	bool hasResultList2();
	VALUE_TO_POINTERS_MAP getResultList1();
	VALUE_TO_POINTERS_MAP getResultList2();
	Reference* getReference1();
	Reference* getReference2();
	bool equals(Result &other);
};
