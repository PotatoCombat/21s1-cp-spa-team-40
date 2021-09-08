#include <vector>

#include "Result.h"
#include "Entity.h"
#include "NamedStatement.h"

#include "catch.hpp"

using namespace std;

NamedStatement namedStmt1("1");
NamedStatement namedStmt2("1");
NamedStatement namedStmt3("2");

vector<string> list1{"1", "2", "3"};
vector<string> list2{"1", "2", "3"};
vector<string> list3{"2", "3"};

TEST_CASE("Result: equals - all the same - returns true") {
	Result result1;
    Result result2;

	result1.setValid(true);
    result2.setValid(true);

	
	result1.setResultList1(&namedStmt1, list1);
    result2.setResultList1(&namedStmt2, list2);

	REQUIRE(result1.equals(result2));
}

TEST_CASE("Result: equals - different isResultValid - returns false") {
	Result result1;
    Result result2;

	result1.setValid(true);
    result2.setValid(false);
	REQUIRE(!result1.equals(result2));
}

TEST_CASE("Result: equals - different entity - returns false") {
    Result result1;
    Result result2;

    result1.setValid(true);
    result2.setValid(true);

    result1.setResultList1(&namedStmt1, list1);
    result2.setResultList1(&namedStmt1, list1);

    result1.setResultList2(&namedStmt1, list1);
    result2.setResultList2(&namedStmt3, list1);

    REQUIRE(!result1.equals(result2));
}

TEST_CASE("Result: equals - different resultList - returns false") {
    Result result1;
    Result result2;

    result1.setValid(true);
    result2.setValid(true);

    result1.setResultList2(&namedStmt1, list1);
    result2.setResultList2(&namedStmt1, list3);

    REQUIRE(!result1.equals(result2));
}