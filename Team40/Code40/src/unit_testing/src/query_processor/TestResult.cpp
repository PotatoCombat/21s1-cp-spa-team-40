#include <vector>

#include "query_processor/Result.h"
#include "query_processor/model/Reference.h"
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

    VALUE_TO_POINTERS_MAP m{{"1", POINTER_SET{}}};
    VALUE_TO_POINTERS_MAP m1{{"1", POINTER_SET{}}};

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

    VALUE_TO_POINTERS_MAP m1{{"1", POINTER_SET{}}};

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

    VALUE_TO_POINTERS_MAP v1{{"1", POINTER_SET{}},
                             {"1", POINTER_SET{make_pair(1, "2")}}};
    VALUE_TO_POINTERS_MAP v2{{"1", POINTER_SET{}}, {"2", POINTER_SET{}}};

    result1.setResultList2(&testResultHelper.ref1, v1);
    result2.setResultList2(&testResultHelper.ref1, v2);

    REQUIRE(!result1.equals(result2));
}
