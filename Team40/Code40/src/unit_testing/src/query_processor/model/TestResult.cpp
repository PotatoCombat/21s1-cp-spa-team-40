#include <vector>

#include "query_processor/model/Reference.h"
#include "query_processor/model/Result.h"
#include "query_processor/ResultTable.h"

#include "catch.hpp"

using namespace std;

struct TestResultHelper {
public:
    Reference ref1;
    Reference ref2;
    Reference ref3;
    Reference ref4;
};

TestResultHelper testResultHelper = {
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "1"),
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "1"),
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "2"),
    Reference(DesignEntityType::CALL, ReferenceType::SYNONYM, "2"),
};


TEST_CASE("Result: equals - all the same - returns true") {
    Result result1;
    Result result2;

    result1.setValid(true);
    result2.setValid(true);

    map<VALUE, VALUE_SET> m{{"1", VALUE_SET{}}};
    map<VALUE, VALUE_SET> m1{{"1", VALUE_SET{}}};

    result1.setResultList1(&testResultHelper.ref1, m1);
    result2.setResultList1(&testResultHelper.ref2, m);

    REQUIRE(result1.equals(result2));
}

TEST_CASE("Result: equals - different isResultValid - returns false") {
    Result result1;
    Result result2;

    result1.setValid(true);
    result2.setValid(false);
    REQUIRE(!result1.equals(result2));
}

TEST_CASE("Result: equals - different reference - returns false") {
    Result result1;
    Result result2;

    result1.setValid(true);
    result2.setValid(true);

    map<VALUE, VALUE_SET> m1{{"1", VALUE_SET{}}};

    result1.setResultList1(&testResultHelper.ref1, m1);
    result2.setResultList1(&testResultHelper.ref1, m1);

    result1.setResultList2(&testResultHelper.ref1, m1);
    result2.setResultList2(&testResultHelper.ref3, m1);

    REQUIRE(!result1.equals(result2));
}

TEST_CASE("Result: equals - different resultList - returns false") {
    Result result1;
    Result result2;

    result1.setValid(true);
    result2.setValid(true);

    map<VALUE, VALUE_SET> v1{{"1", VALUE_SET{}}, {"3", VALUE_SET{"2"}}};
    map<VALUE, VALUE_SET> v2{{"1", VALUE_SET{}}, {"2", VALUE_SET{}}};

    result1.setResultList2(&testResultHelper.ref1, v1);
    result2.setResultList2(&testResultHelper.ref1, v2);

    REQUIRE(!result1.equals(result2));
}
