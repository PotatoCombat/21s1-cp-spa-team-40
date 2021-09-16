#include "catch.hpp"

#include "query_processor/Abstractions.h"
#include "query_processor/parser/XEntParser.h"

struct TestXEntParser {
    static Reference STMT;
    static Reference UNDECLARED_STMT;
    static Reference PROCEDURE;
    static Reference VARIABLE;
    static Reference UNDECLARED_VARIABLE;
    static Reference WILDCARD;
    static Reference CONSTANT_STMT;
    static Reference CONSTANT_PROCEDURE;
    static Reference CONSTANT_VARIABLE;
    static Clause *createModifiesSClause(Reference r1, Reference r2);
    static Clause *createModifiesPClause(Reference r1, Reference r2);
};

Reference TestXEntParser::STMT =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
Reference TestXEntParser::UNDECLARED_STMT =
    Reference(DesignEntityType::STMT, ReferenceType::SYNONYM, "a");
Reference TestXEntParser::PROCEDURE =
    Reference(DesignEntityType::PROCEDURE, ReferenceType::SYNONYM, "p");
Reference TestXEntParser::VARIABLE =
    Reference(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "v");
Reference TestXEntParser::UNDECLARED_VARIABLE = Reference(
    DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "somevariable");
Reference TestXEntParser::WILDCARD =
    Reference(DesignEntityType::VARIABLE, ReferenceType::WILDCARD, "_");
Reference TestXEntParser::CONSTANT_STMT =
    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
Reference TestXEntParser::CONSTANT_PROCEDURE = Reference(
    DesignEntityType::PROCEDURE, ReferenceType::CONSTANT, "procedure");
Reference TestXEntParser::CONSTANT_VARIABLE = Reference(
    DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "variable");

Clause *TestXEntParser::createModifiesSClause(Reference r1, Reference r2) {
    return new Clause(ClauseType::MODIFIES_S, r1, r2);
}

Clause *TestXEntParser::createModifiesPClause(Reference r1, Reference r2) {
    return new Clause(ClauseType::MODIFIES_P, r1, r2);
}

TEST_CASE("XEntParser: first argument cannot be wildcard") {
    vector<Reference *> declList;
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

    ClsTuple tup = make_tuple("Modifies", "_", "v");
    XEntParser p(tup, declList, deHelper, clsHelper);

    REQUIRE_THROWS_AS(p.parse(), ValidityError);
}

TEST_CASE(
    "XEntParser: differentiate between a statement or procedure constant") {
    vector<Reference *> declList;
    declList.push_back(&TestXEntParser::STMT);
    declList.push_back(&TestXEntParser::PROCEDURE);
    declList.push_back(&TestXEntParser::VARIABLE);
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

    SECTION("test quoted should become ModifiesP") {
        Clause *expected = TestXEntParser::createModifiesPClause(
            TestXEntParser::CONSTANT_PROCEDURE, TestXEntParser::VARIABLE);
        ClsTuple tup = make_tuple("Modifies", "\"procedure\"", "v");
        XEntParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }

    SECTION("test integer should become ModifiesS") {
        Clause *expected = TestXEntParser::createModifiesSClause(
            TestXEntParser::CONSTANT_STMT, TestXEntParser::VARIABLE);
        ClsTuple tup = make_tuple("Modifies", "1", "v");
        XEntParser p(tup, declList, deHelper, clsHelper);
        Clause *actual = p.parse();

        REQUIRE(actual->equals(*expected));
        delete expected;
        delete actual;
    }
}

TEST_CASE("XEntParser: second argument wildcard") {
    vector<Reference *> declList;
    declList.push_back(&TestXEntParser::STMT);
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

    Clause *expected = TestXEntParser::createModifiesSClause(
        TestXEntParser::STMT, TestXEntParser::WILDCARD);
    ClsTuple tup = make_tuple("Modifies", "s", "_");
    XEntParser p(tup, declList, deHelper, clsHelper);
    Clause *actual = p.parse();

    REQUIRE(actual->equals(*expected));
    delete expected;
    delete actual;
}

TEST_CASE("XEntParser: second argument constant") {
    vector<Reference*> declList;
    declList.push_back(&TestXEntParser::STMT);
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

    Clause* expected = TestXEntParser::createModifiesSClause(
        TestXEntParser::STMT, TestXEntParser::CONSTANT_VARIABLE);
    ClsTuple tup = make_tuple("Modifies", "s", "\"variable\"");
    XEntParser p(tup, declList, deHelper, clsHelper);
    Clause* actual = p.parse();

    REQUIRE(actual->equals(*expected));
    delete expected;
    delete actual;
}

TEST_CASE("XEntParer: undeclared synonym") {
    vector<Reference*> declList;
    declList.push_back(&TestXEntParser::STMT);
    declList.push_back(&TestXEntParser::PROCEDURE);
    declList.push_back(&TestXEntParser::VARIABLE);
    DesignEntityTypeHelper deHelper;
    ClauseTypeHelper clsHelper;

    SECTION("test undeclared first declared second") {
        ClsTuple tup = make_tuple("Modifies", "a", "v");
        XEntParser p(tup, declList, deHelper, clsHelper);
        REQUIRE_THROWS_AS(p.parse(), ValidityError);
    }

    SECTION("test declared first undeclared second") {
        ClsTuple tup = make_tuple("Modifies", "s", "somevariable");
        XEntParser p(tup, declList, deHelper, clsHelper);
        REQUIRE_THROWS_AS(p.parse(), ValidityError);
    }
}
