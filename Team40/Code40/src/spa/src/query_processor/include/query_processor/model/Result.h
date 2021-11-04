# pragma once

#include <vector>

#include "query_processor/model/Reference.h"
#include "query_processor/ResultTable.h"

using namespace std;

/** 
* Represents the intermediate result returned by each clause
*/
class Result {
private:
    bool isValid; // false if a True/False clause returns false, true otherwise
    Reference *reference1;
    Reference *reference2;
    map<VALUE, VALUE_SET> resultList1;
    map<VALUE, VALUE_SET> resultList2;

public:
    Result() : isValid(true), reference1(NULL), reference2(NULL) {}
    void setValid(bool isValid);
    bool isResultValid();
    void setResultList1(Reference *reference, map<VALUE, VALUE_SET> resultList);
    void setResultList2(Reference *reference, map<VALUE, VALUE_SET> resultList);
    bool hasResultList1();
    bool hasResultList2();
    map<VALUE, VALUE_SET> getResultList1();
    map<VALUE, VALUE_SET> getResultList2();
    Reference *getReference1();
    Reference *getReference2();
    bool equals(Result &other);
};
