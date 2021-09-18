#include <vector>

#include "query_processor/Result.h"
#include "query_processor/model/Reference.h"
#include "query_processor/ValueToPointersMap.h"

#include "catch.hpp"

using namespace std;

struct TestResultHelper {
public:
    Reference ref1;
    Reference ref2;
    Reference ref3;
    Reference ref4;
    ValueToPointersMap m1;
    ValueToPointersMap m2;
    ValueToPointersMap m3;
};

TestResultHelper testResultHelper = {
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "1"),
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "1"),
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "2"),
    Reference(DesignEntityType::CALL, ReferenceType::SYNONYM, "2"),
    ValueToPointersMap("1", POINTER_SET{}),
    ValueToPointersMap("2", POINTER_SET{}),
    ValueToPointersMap("1", POINTER_SET{make_pair(1, "2")})};


TEST_CASE("Result: equals - all the same - returns true") {
    Result result1;
    Result result2;

    result1.setValid(true);
    result2.setValid(true);

    ValueToPointersMap m("1", POINTER_SET{});

    vector<ValueToPointersMap> v1{testResultHelper.m1};
    vector<ValueToPointersMap> v2{testResultHelper.m1};

    result1.setResultList1(&testResultHelper.ref1, v1);
    result2.setResultList1(&testResultHelper.ref2, v2);

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

    vector<ValueToPointersMap> v1{testResultHelper.m1};

    result1.setResultList1(&testResultHelper.ref1, v1);
    result2.setResultList1(&testResultHelper.ref1, v1);

    result1.setResultList2(&testResultHelper.ref1, v1);
    result2.setResultList2(&testResultHelper.ref3, v1);

    REQUIRE(!result1.equals(result2));
}

TEST_CASE("Result: equals - different resultList - returns false") {
    Result result1;
    Result result2;

    result1.setValid(true);
    result2.setValid(true);

    vector<ValueToPointersMap> v1{testResultHelper.m1, testResultHelper.m3};
    vector<ValueToPointersMap> v2{testResultHelper.m1, testResultHelper.m2};

    result1.setResultList2(&testResultHelper.ref1, v1);
    result2.setResultList2(&testResultHelper.ref1, v2);

    REQUIRE(!result1.equals(result2));
}
