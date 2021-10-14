#include "catch.hpp"

#include "query_processor/model/Clause.h"
#include "query_processor/model/Reference.h"

using namespace std;

struct TestClause {
    static Reference R1;
    static Reference R2;
    static Clause *createFollowsClause();
    static Clause *createParentClause();
};

Reference TestClause::R1 =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s",
              ReferenceAttribute::INTEGER);
Reference TestClause::R2 =
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "_",
              ReferenceAttribute::INTEGER);

Clause *TestClause::createFollowsClause() {
    return new Clause(ClauseType::FOLLOWS, R1, R2);
}

Clause *TestClause::createParentClause() {
    Reference ref1 = R1;
    Reference ref2 = R2;
    return new Clause(ClauseType::PARENT, ref1, ref2);
}

TEST_CASE("Clause: get methods") {
    Clause *clause = TestClause::createFollowsClause();

    REQUIRE(clause->getType() == ClauseType::FOLLOWS);
    REQUIRE(TestClause::R1.equals(*(clause->getFirstReference())));
    REQUIRE(TestClause::R2.equals(*(clause->getSecondReference())));
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
