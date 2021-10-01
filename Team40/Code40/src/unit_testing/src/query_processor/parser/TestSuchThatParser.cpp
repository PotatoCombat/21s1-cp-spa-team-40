#include "catch.hpp"

#include "query_processor/Abstractions.h"
#include "query_processor/parser/SuchThatParser.h"

struct TestSuchThatParser {
    static Reference DECLARED_STMT;
    static Reference DECLARED_ASSIGN;
    static Reference DECLARED_VARIABLE;
    static Reference DECLARED_PROCEDURE;
    static Reference WILDCARD_STMT;
    static Reference WILDCARD_VARIABLE;
    static Reference CONSTANT_STMT_1;
    static Reference CONSTANT_STMT_4;
    static Reference CONSTANT_VARIABLE;
    static Reference CONSTANT_PROCEDURE;
    static vector<Reference *> DECLARATIONS;
    static Clause *createParent(Reference r1, Reference r2);
    static Clause *createModifiesS(Reference r1, Reference r2);
    static Clause *createModifiesP(Reference r1, Reference r2);
};

Reference TestSuchThatParser::DECLARED_STMT =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "statement");
Reference TestSuchThatParser::DECLARED_ASSIGN =
    Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
Reference TestSuchThatParser::DECLARED_VARIABLE = Reference(
    DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "foodVariable");
Reference TestSuchThatParser::DECLARED_PROCEDURE =
    Reference(DesignEntityType::PROCEDURE, ReferenceType::SYNONYM, "PROCEDURE");
Reference TestSuchThatParser::WILDCARD_STMT =
    Reference(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
Reference TestSuchThatParser::WILDCARD_VARIABLE =
    Reference(DesignEntityType::VARIABLE, ReferenceType::WILDCARD, "_");
Reference TestSuchThatParser::CONSTANT_STMT_1 =
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
Reference TestSuchThatParser::CONSTANT_STMT_4 =
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
Reference TestSuchThatParser::CONSTANT_VARIABLE =
    Reference(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "value");
Reference TestSuchThatParser::CONSTANT_PROCEDURE = Reference(
    DesignEntityType::PROCEDURE, ReferenceType::CONSTANT, "procedur3");

vector<Reference *> TestSuchThatParser::DECLARATIONS = {
    &DECLARED_STMT, &DECLARED_ASSIGN, &DECLARED_VARIABLE, &DECLARED_PROCEDURE};

Clause *TestSuchThatParser::createParent(Reference r1, Reference r2) {
    return new Clause(ClauseType::PARENT, r1, r2);
}

Clause *TestSuchThatParser::createModifiesS(Reference r1, Reference r2) {
    return new Clause(ClauseType::MODIFIES_S, r1, r2);
}

Clause *TestSuchThatParser::createModifiesP(Reference r1, Reference r2) {
    return new Clause(ClauseType::MODIFIES_P, r1, r2);
}

TEST_CASE("SuchThatParser: parse parent clause - valid arguments") {
    SuchThatParser p;
    p.initReferences(TestSuchThatParser::DECLARATIONS);

    SECTION("test wildcard wildcard") {
        Clause *expected =
            TestSuchThatParser::createParent(TestSuchThatParser::WILDCARD_STMT,
                                             TestSuchThatParser::WILDCARD_STMT);
        ClsTuple tup = make_tuple("Parent", "_", "_");
        Clause *actual = p.parse(tup);

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test constant constant") {
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::CONSTANT_STMT_1,
            TestSuchThatParser::CONSTANT_STMT_4);
        ClsTuple tup = make_tuple("Parent", "1", "4");
        Clause *actual = p.parse(tup);

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test wildcard/constant") {
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::WILDCARD_STMT,
            TestSuchThatParser::CONSTANT_STMT_1);
        ClsTuple tup = make_tuple("Parent", "_", "1");
        Clause *actual = p.parse(tup);

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParent(
            TestSuchThatParser::CONSTANT_STMT_1,
            TestSuchThatParser::WILDCARD_STMT);
        tup = make_tuple("Parent", "1", "_");
        actual = p.parse(tup);

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test wildcard/synonym") {
        Clause *expected =
            TestSuchThatParser::createParent(TestSuchThatParser::WILDCARD_STMT,
                                             TestSuchThatParser::DECLARED_STMT);
        ClsTuple tup = make_tuple("Parent", "_", "statement");
        Clause *actual = p.parse(tup);

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected =
            TestSuchThatParser::createParent(TestSuchThatParser::DECLARED_STMT,
                                             TestSuchThatParser::WILDCARD_STMT);
        tup = make_tuple("Parent", "statement", "_");
        actual = p.parse(tup);

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test constant/synonym") {
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::CONSTANT_STMT_1,
            TestSuchThatParser::DECLARED_STMT);
        ClsTuple tup = make_tuple("Parent", "1", "statement");
        Clause *actual = p.parse(tup);

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createParent(
            TestSuchThatParser::DECLARED_STMT,
            TestSuchThatParser::CONSTANT_STMT_1);
        tup = make_tuple("Parent", "statement", "1");
        actual = p.parse(tup);

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test synonym/synonym") {
        Clause *expected = TestSuchThatParser::createParent(
            TestSuchThatParser::DECLARED_STMT,
            TestSuchThatParser::DECLARED_ASSIGN);
        ClsTuple tup = make_tuple("Parent", "statement", "a");
        Clause *actual = p.parse(tup);

        REQUIRE(expected->equals(*actual));
        delete expected;
        delete actual;
    }
};

TEST_CASE("SuchThatParser: parse parent clause - invalid arguments") {
    SuchThatParser p;
    p.initReferences(TestSuchThatParser::DECLARATIONS);

    SECTION("TEST FAIL: quoted argument") {
        SECTION("integer/quoted") {
            ClsTuple tup = make_tuple("Parent", "1", "\"C4\"");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);

            tup = make_tuple("Parent", "\"C4\"", "1");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
        }

        SECTION("wildcard/quoted") {
            ClsTuple tup = make_tuple("Parent", "_", "\"C4\"");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);

            tup = make_tuple("Parent", "\"C4\"", "_");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
        }

        SECTION("synonym/quoted") {
            ClsTuple tup = make_tuple("Parent", "statement", "\"C4\"");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);

            tup = make_tuple("Parent", "\"C4\"", "statement");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
        }
    }

    SECTION("TEST FAIL: wrong type synonym (non-statement)") {
        SECTION("first synonym non-statement") {
            ClsTuple tup = make_tuple("Parent", "foodVariable", "a");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);

            tup = make_tuple("Parent", "PROCEDURE", "a");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
        }

        SECTION("second synonym non-statement") {
            ClsTuple tup = make_tuple("Parent", "a", "foodVariable");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);

            tup = make_tuple("Parent", "a", "PROCEDURE");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
        }
    }

    SECTION("TEST FAIL: undeclared synonym") {
        SECTION("first synonym undeclared") {
            ClsTuple tup = make_tuple("Parent", "statement", "aaaaaa");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
        }

        SECTION("second synonym undeclared") {
            ClsTuple tup = make_tuple("Parent", "aaaaaa", "statement");
            REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
        }
    }
};

TEST_CASE("SuchThatParser: parse modifies clause - valid arguments") {
    SuchThatParser p;
    p.initReferences(TestSuchThatParser::DECLARATIONS);

    SECTION("int wildcard / int name / int variable creates ModifiesS") {
        Reference stmt = TestSuchThatParser::CONSTANT_STMT_1;

        Clause *expected = TestSuchThatParser::createModifiesS(
            stmt, TestSuchThatParser::WILDCARD_VARIABLE);
        ClsTuple tup = make_tuple("Modifies", "1", "_");
        Clause *actual = p.parse(tup);
        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createModifiesS(
            stmt, TestSuchThatParser::CONSTANT_VARIABLE);
        tup = make_tuple("Modifies", "1", "\"value\"");
        actual = p.parse(tup);
        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createModifiesS(
            stmt, TestSuchThatParser::DECLARED_VARIABLE);
        tup = make_tuple("Modifies", "1", "foodVariable");
        actual = p.parse(tup);
        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("name wildcard / name name / name variable creates ModifiesP") {
        Reference proc = TestSuchThatParser::CONSTANT_PROCEDURE;

        Clause *expected = TestSuchThatParser::createModifiesP(
            proc, TestSuchThatParser::WILDCARD_VARIABLE);
        ClsTuple tup = make_tuple("Modifies", "\"procedur3\"", "_");
        Clause *actual = p.parse(tup);
        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createModifiesP(
            proc, TestSuchThatParser::CONSTANT_VARIABLE);
        tup = make_tuple("Modifies", "\"procedur3\"", "\"value\"");
        actual = p.parse(tup);
        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createModifiesP(
            proc, TestSuchThatParser::DECLARED_VARIABLE);
        tup = make_tuple("Modifies", "\"procedur3\"", "foodVariable");
        actual = p.parse(tup);
        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("synonym wildcard/name/variable") {
        Clause *expected = TestSuchThatParser::createModifiesS(
            TestSuchThatParser::DECLARED_ASSIGN,
            TestSuchThatParser::WILDCARD_VARIABLE);
        ClsTuple tup = make_tuple("Modifies", "a", "_");
        Clause *actual = p.parse(tup);
        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createModifiesS(
            TestSuchThatParser::DECLARED_ASSIGN,
            TestSuchThatParser::CONSTANT_VARIABLE);
        tup = make_tuple("Modifies", "a", "\"value\"");
        actual = p.parse(tup);
        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;

        expected = TestSuchThatParser::createModifiesS(
            TestSuchThatParser::DECLARED_ASSIGN,
            TestSuchThatParser::DECLARED_VARIABLE);
        tup = make_tuple("Modifies", "a", "foodVariable");
        actual = p.parse(tup);
        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }
};

 TEST_CASE("SuchThatParser: parse modifies clause - invalid arguments") {
    SuchThatParser p;
    p.initReferences(TestSuchThatParser::DECLARATIONS);

    SECTION("TEST FAIL: first argument cannot be wildcard") {
        ClsTuple tup = make_tuple("Modifies", "_", "_");
        REQUIRE_THROWS_AS(p.parse(tup), ValidityError);

        tup = make_tuple("Modifies", "_", "foodVariable");
        REQUIRE_THROWS_AS(p.parse(tup), ValidityError);

        tup = make_tuple("Modifies", "_", "\"value\"");
        REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
    }

    SECTION("TEST FAIL: first argument cannot be variable type") {
        ClsTuple tup = make_tuple("Modifies", "foodVariable", "\"value\"");
        REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
    }

    SECTION("TEST FAIL: second argument cannot be integer") {
        ClsTuple tup = make_tuple("Modifies", "\"procedur3\"", "1");
        REQUIRE_THROWS_AS(p.parse(tup), ValidityError);

        tup = make_tuple("Modifies", "1", "1");
        REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
    }

    SECTION("TEST FAIL: second argument cannot be non-variable type") {
        ClsTuple tup = make_tuple("Modifies", "statement", "PROCEDURE");
        REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
        
        tup = make_tuple("Modifies", "statement", "a");
        REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
    }

    SECTION("TEST FAIL: undeclared synonym") {
        ClsTuple tup = make_tuple("Modifies", "statement", "undeclared");
        REQUIRE_THROWS_AS(p.parse(tup), ValidityError);

        tup = make_tuple("Modifies", "procedure", "foodVariable");
        REQUIRE_THROWS_AS(p.parse(tup), ValidityError);
    }
};
