#include "catch.hpp"

#include "query_processor/model/Clause.h"

using namespace std;

struct TestClause {
    static Reference *R1;
    static Reference *R2;
    static Reference *R3;
    static Clause *createFollowsClause();
    static Clause *createParentClause();
    static Clause *createPatternClause();
};

Reference *TestClause::R1 =
    new Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s",
                  ReferenceAttribute::INTEGER);
Reference *TestClause::R2 =
    new Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "_",
                  ReferenceAttribute::INTEGER);
Reference *TestClause::R3 =
    new Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a",
                  ReferenceAttribute::INTEGER);

Clause *TestClause::createFollowsClause() {
    return new Clause(ClauseType::FOLLOWS, *R1, *R2);
}

Clause *TestClause::createParentClause() {
    return new Clause(ClauseType::PARENT, *R1, *R2);
}

Clause *TestClause::createPatternClause() {
    return new Clause(*R3, *R2, {"x", "+", "y"}, false);
}

TEST_CASE("Clause: get methods") {
    Clause *clause = TestClause::createFollowsClause();

    REQUIRE(clause->getType() == ClauseType::FOLLOWS);
    REQUIRE(TestClause::R1->equals(*(clause->getFirstReference())));
    REQUIRE(TestClause::R2->equals(*(clause->getSecondReference())));
    delete clause;

    clause = TestClause::createPatternClause();
    REQUIRE(clause->getType() == ClauseType::PATTERN);
    REQUIRE(TestClause::R3->equals(*(clause->getFirstReference())));
    REQUIRE(TestClause::R2->equals(*(clause->getSecondReference())));
    REQUIRE(clause->getPattern() == vector<string>{"x", "+", "y"});
    REQUIRE(clause->isExactMatch() == false);
    delete clause;
}

TEST_CASE("Clause: equals") {
    Reference ref1(DesignEntityType::STMT, ReferenceType::CONSTANT, "1",
                   ReferenceAttribute::INTEGER);
    Reference ref2(DesignEntityType::STMT, ReferenceType::CONSTANT, "1",
                   ReferenceAttribute::INTEGER);
    Reference ref3(DesignEntityType::STMT, ReferenceType::CONSTANT, "2",
                   ReferenceAttribute::INTEGER);
    Reference ref4(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "x",
                   ReferenceAttribute::NAME);
    Reference ref5(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "x",
                   ReferenceAttribute::NAME);
    Reference ref6(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "x",
                   ReferenceAttribute::NAME);

    // all match
    Clause clause1(ClauseType::USES_S, ref1, ref4);
    Clause clause2(ClauseType::USES_S, ref2, ref5);
    REQUIRE(clause1.equals(clause2));
    // different type
    Clause clause3(ClauseType::MODIFIES_S, ref1, ref4);
    Clause clause4(ClauseType::USES_S, ref2, ref5);
    REQUIRE(!clause3.equals(clause4));
    // different first reference
    Clause clause5(ClauseType::USES_S, ref1, ref4);
    Clause clause6(ClauseType::USES_S, ref3, ref5);
    REQUIRE(!clause5.equals(clause6));
    // different second reference
    Clause clause7(ClauseType::USES_S, ref1, ref4);
    Clause clause8(ClauseType::USES_S, ref2, ref6);
    REQUIRE(!clause7.equals(clause8));
}

TEST_CASE("Clause: copy") {
    Clause *cls = TestClause::createParentClause();
    Clause *clsCopy = cls->copy();

    REQUIRE(cls->equals(*clsCopy));

    delete cls;
    delete clsCopy;
}
