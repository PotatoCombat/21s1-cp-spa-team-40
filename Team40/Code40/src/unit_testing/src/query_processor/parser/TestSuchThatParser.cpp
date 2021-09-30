#include "catch.hpp"

#include "query_processor/Abstractions.h"
#include "query_processor/parser/SuchThatParser.h"

struct TestSuchThatParser {
    static Reference DECLARED_STMT;
    static Reference DECLARED_ASSIGN;
    static Reference DECLARED_VARIABLE;
    static Reference DECLARED_PROCEDURE;
    static Reference WILDCARD;
    static Reference CONSTANT_STMT_1;
    static Reference CONSTANT_STMT_4;
    static Clause *createParent(Reference r1, Reference r2);
    static Clause *createModifiesS(Reference r1, Reference r2);
    static Clause *createModifiesP(Reference r1, Reference r2);
};

Reference TestSuchThatParser::DECLARED_STMT =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "statement");
Reference TestSuchThatParser::DECLARED_ASSIGN =
    Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
Reference TestSuchThatParser::DECLARED_VARIABLE =
    Reference(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "food");
Reference TestSuchThatParser::DECLARED_PROCEDURE =
    Reference(DesignEntityType::PROCEDURE, ReferenceType::SYNONYM, "PROCEDURE");
Reference TestSuchThatParser::WILDCARD =
    Reference(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
Reference TestSuchThatParser::CONSTANT_STMT_1 =
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
Reference TestSuchThatParser::CONSTANT_STMT_4 =
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");

Clause *TestSuchThatParser::createParent(Reference r1, Reference r2) {
    return new Clause(ClauseType::PARENT, r1, r2);
}

Clause *TestSuchThatParser::createModifiesS(Reference r1, Reference r2) {
    return new Clause(ClauseType::MODIFIES_S, r1, r2);
}

Clause *TestSuchThatParser::createModifiesP(Reference r1, Reference r2) {
    return new Clause(ClauseType::MODIFIES_P, r1, r2);
}

TEST_CASE("SuchThatParser: parse parent clause") {
    vector<Reference *> declList;
    declList.push_back(&TestSuchThatParser::DECLARED_STMT);
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

    SECTION("test wildcard wildcard") {
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::WILDCARD, TestSuchThatParser::WILDCARD);
        ClsTuple tup = make_tuple("Parent", "_", "_");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test constant constant") {
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::CONSTANT_STMT_4);
        ClsTuple tup = make_tuple("Parent", "1", "4");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test wildcard/constant") {
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::WILDCARD, TestSuchThatParser::CONSTANT_STMT_1);
        ClsTuple tup = make_tuple("Parent", "_", "1");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParent(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::WILDCARD);
        tup = make_tuple("Parent", "1", "_");
        p = SuchThatParser(tup, declList, deHelper, clsHelper);
        actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test wildcard/synonym") {
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::WILDCARD, TestSuchThatParser::CONSTANT_STMT_1);
        ClsTuple tup = make_tuple("Parent", "_", "1");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParent(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::WILDCARD);
        tup = make_tuple("Parent", "1", "_");
        p = SuchThatParser(tup, declList, deHelper, clsHelper);
        actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test constant/synonym") {
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::DECLARED_STMT);
        ClsTuple tup = make_tuple("Parent", "1", "s");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParent(
            TestSuchThatParser::DECLARED_STMT, TestSuchThatParser::CONSTANT_STMT_1);
        tup = make_tuple("Parent", "s", "1");
        p = SuchThatParser(tup, declList, deHelper, clsHelper);
        actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test synonym/synonym") {
        declList.push_back(&TestSuchThatParser::DECLARED_ASSIGN);
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::DECLARED_STMT,
            TestSuchThatParser::DECLARED_ASSIGN);
        ClsTuple tup = make_tuple("Parent", "s", "a");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(expected->equals(*actual));
        delete expected;
        delete actual;
    }

    SECTION("test undeclared synonym - FAIL") {
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::DECLARED_STMT,
            TestSuchThatParser::DECLARED_ASSIGN);
        ClsTuple tup = make_tuple("Parent", "s", "a");
        SuchThatParser p(tup, declList, deHelper, clsHelper);

        REQUIRE_THROWS_AS(p.parse(), ValidityError);
    }
};

TEST_CASE("SuchThatParser: parse modifies statement clause") {
    vector<Reference *> declList;
    declList.push_back(&TestSuchThatParser::DECLARED_STMT);
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

    SECTION("test wildcard wildcard") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::WILDCARD, TestSuchThatParser::WILDCARD);
        ClsTuple tup = make_tuple("Parent", "_", "_");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test constant constant") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::CONSTANT_STMT_4);
        ClsTuple tup = make_tuple("Parent", "1", "4");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test wildcard/constant") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::WILDCARD, TestSuchThatParser::CONSTANT_STMT_1);
        ClsTuple tup = make_tuple("Parent", "_", "1");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::WILDCARD);
        tup = make_tuple("Parent", "1", "_");
        p = SuchThatParser(tup, declList, deHelper, clsHelper);
        actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test wildcard/synonym") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::WILDCARD, TestSuchThatParser::CONSTANT_STMT_1);
        ClsTuple tup = make_tuple("Parent", "_", "1");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::WILDCARD);
        tup = make_tuple("Parent", "1", "_");
        p = SuchThatParser(tup, declList, deHelper, clsHelper);
        actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test constant/synonym") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::DECLARED_STMT);
        ClsTuple tup = make_tuple("Parent", "1", "s");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::DECLARED_STMT, TestSuchThatParser::CONSTANT_STMT_1);
        tup = make_tuple("Parent", "s", "1");
        p = SuchThatParser(tup, declList, deHelper, clsHelper);
        actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test synonym/synonym") {
        declList.push_back(&TestSuchThatParser::DECLARED_ASSIGN);
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::DECLARED_STMT,
            TestSuchThatParser::DECLARED_ASSIGN);
        ClsTuple tup = make_tuple("Parent", "s", "a");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(expected->equals(*actual));
        delete expected;
        delete actual;
    }

    SECTION("test undeclared synonym - FAIL") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::DECLARED_STMT,
            TestSuchThatParser::DECLARED_ASSIGN);
        ClsTuple tup = make_tuple("Parent", "s", "a");
        SuchThatParser p(tup, declList, deHelper, clsHelper);

        REQUIRE_THROWS_AS(p.parse(), ValidityError);
    }
};

TEST_CASE("SuchThatParser: parse modifies procedure clause") {
    vector<Reference *> declList;
    declList.push_back(&TestSuchThatParser::DECLARED_STMT);
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

    SECTION("test wildcard wildcard") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::WILDCARD, TestSuchThatParser::WILDCARD);
        ClsTuple tup = make_tuple("Parent", "_", "_");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test constant constant") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::CONSTANT_STMT_4);
        ClsTuple tup = make_tuple("Parent", "1", "4");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test wildcard/constant") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::WILDCARD, TestSuchThatParser::CONSTANT_STMT_1);
        ClsTuple tup = make_tuple("Parent", "_", "1");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::WILDCARD);
        tup = make_tuple("Parent", "1", "_");
        p = SuchThatParser(tup, declList, deHelper, clsHelper);
        actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test wildcard/synonym") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::WILDCARD, TestSuchThatParser::CONSTANT_STMT_1);
        ClsTuple tup = make_tuple("Parent", "_", "1");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::WILDCARD);
        tup = make_tuple("Parent", "1", "_");
        p = SuchThatParser(tup, declList, deHelper, clsHelper);
        actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test constant/synonym") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::CONSTANT_STMT_1, TestSuchThatParser::DECLARED_STMT);
        ClsTuple tup = make_tuple("Parent", "1", "s");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::DECLARED_STMT, TestSuchThatParser::CONSTANT_STMT_1);
        tup = make_tuple("Parent", "s", "1");
        p = SuchThatParser(tup, declList, deHelper, clsHelper);
        actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test synonym/synonym") {
        declList.push_back(&TestSuchThatParser::DECLARED_ASSIGN);
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::DECLARED_STMT,
            TestSuchThatParser::DECLARED_ASSIGN);
        ClsTuple tup = make_tuple("Parent", "s", "a");
        SuchThatParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(expected->equals(*actual));
        delete expected;
        delete actual;
    }

    SECTION("test undeclared synonym - FAIL") {
        Clause *expected = TestSuchThatParser::createParentClause(
            TestSuchThatParser::DECLARED_STMT,
            TestSuchThatParser::DECLARED_ASSIGN);
        ClsTuple tup = make_tuple("Parent", "s", "a");
        SuchThatParser p(tup, declList, deHelper, clsHelper);

        REQUIRE_THROWS_AS(p.parse(), ValidityError);
    }
};
