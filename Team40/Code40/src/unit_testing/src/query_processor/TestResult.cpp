#include <vector>

#include "query_processor/Result.h"
#include "query_processor/model/Reference.h"

#include "catch.hpp"

using namespace std;

struct TestResultHelper {
public:
    Reference ref1;
    Reference ref2;
    Reference ref3;
    Reference ref4;
    vector<string> list1;
    vector<string> list2;
    vector<string> list3;
};

TestResultHelper testResultHelper = {
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "1"),
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "1"),
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "2"),
    Reference(DesignEntityType::CALL, ReferenceType::SYNONYM, "2"),
    vector<string>{"1", "2", "3"},
    vector<string>{"1", "2", "3"},
    vector<string>{"2", "3"}};

TEST_CASE("Result: equals - all the same - returns true") {
    Result result1;
    Result result2;

    result1.setValid(true);
    result2.setValid(true);

    result1.setResultList1(&testResultHelper.ref1, testResultHelper.list1);
    result2.setResultList1(&testResultHelper.ref2, testResultHelper.list2);

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

    result1.setResultList1(&testResultHelper.ref1, testResultHelper.list1);
    result2.setResultList1(&testResultHelper.ref1, testResultHelper.list1);

    result1.setResultList2(&testResultHelper.ref1, testResultHelper.list1);
    result2.setResultList2(&testResultHelper.ref3, testResultHelper.list1);

    REQUIRE(!result1.equals(result2));
}

TEST_CASE("Result: equals - different resultList - returns false") {
    Result result1;
    Result result2;

    result1.setValid(true);
    result2.setValid(true);

    result1.setResultList2(&testResultHelper.ref1, testResultHelper.list1);
    result2.setResultList2(&testResultHelper.ref1, testResultHelper.list3);

    REQUIRE(!result1.equals(result2));
}
