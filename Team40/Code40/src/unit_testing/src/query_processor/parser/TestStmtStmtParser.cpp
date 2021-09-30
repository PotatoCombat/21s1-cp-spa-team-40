//#include "catch.hpp"
//
//#include "query_processor/Abstractions.h"
//#include "query_processor/parser/StmtStmtParser.h"
//
//struct TestStmtStmtParser {
//    static Reference DECLARED_SYN;
//    static Reference DECLARED_SYN1;
//    static Reference WILDCARD;
//    static Reference CONSTANT1;
//    static Reference CONSTANT4;
//    static Clause *createParentClause(Reference r1, Reference r2);
//};
//
//Reference TestStmtStmtParser::DECLARED_SYN =
//    Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "s");
//
//Reference TestStmtStmtParser::DECLARED_SYN1 =
//    Reference(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
//
//Reference TestStmtStmtParser::WILDCARD =
//    Reference(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
//
//Reference TestStmtStmtParser::CONSTANT1 =
//    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "1");
//
//Reference TestStmtStmtParser::CONSTANT4 =
//    Reference(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
//
//Clause *TestStmtStmtParser::createParentClause(Reference r1, Reference r2) {
//    return new Clause(ClauseType::PARENT, r1, r2);
//}
//
//TEST_CASE("StmtStmtParser") {
//    vector<Reference *> declList;
//    declList.push_back(&TestStmtStmtParser::DECLARED_SYN);
//    DesignEntityTypeHelper deHelper;
//    ClauseTypeHelper clsHelper;
//
//    SECTION("test wildcard wildcard") {
//        Clause *expected = TestStmtStmtParser::createParentClause(
//            TestStmtStmtParser::WILDCARD, TestStmtStmtParser::WILDCARD);
//        ClsTuple tup = make_tuple("Parent", "_", "_");
//        StmtStmtParser p(tup, declList, deHelper, clsHelper);
//        Clause* actual = p.parse();
//
//        REQUIRE(actual->equals(*expected));
//        delete expected;
//        delete actual;
//    }
//
//    SECTION("test constant constant") {
//        Clause* expected = TestStmtStmtParser::createParentClause(
//            TestStmtStmtParser::CONSTANT1, TestStmtStmtParser::CONSTANT4);
//        ClsTuple tup = make_tuple("Parent", "1", "4");
//        StmtStmtParser p(tup, declList, deHelper, clsHelper);
//        Clause* actual = p.parse();
//
//        REQUIRE(actual->equals(*expected));
//        delete expected;
//        delete actual;
//    }
//
//    SECTION("test wildcard/constant") {
//        Clause* expected = TestStmtStmtParser::createParentClause(
//            TestStmtStmtParser::WILDCARD, TestStmtStmtParser::CONSTANT1);
//        ClsTuple tup = make_tuple("Parent", "_", "1");
//        StmtStmtParser p(tup, declList, deHelper, clsHelper);
//        Clause* actual = p.parse();
//
//        REQUIRE(actual->equals(*expected));
//        delete expected;
//        delete actual;
//
//        expected = TestStmtStmtParser::createParentClause(
//            TestStmtStmtParser::CONSTANT1, TestStmtStmtParser::WILDCARD);
//        tup = make_tuple("Parent", "1", "_");
//        p = StmtStmtParser(tup, declList, deHelper, clsHelper);
//        actual = p.parse();
//
//        REQUIRE(actual->equals(*expected));
//        delete expected;
//        delete actual;
//    }
//
//    SECTION("test constant/synonym") {
//        Clause* expected = TestStmtStmtParser::createParentClause(
//            TestStmtStmtParser::CONSTANT1, TestStmtStmtParser::DECLARED_SYN);
//        ClsTuple tup = make_tuple("Parent", "1", "s");
//        StmtStmtParser p(tup, declList, deHelper, clsHelper);
//        Clause* actual = p.parse();
//
//        REQUIRE(actual->equals(*expected));
//        delete expected;
//        delete actual;
//
//        expected = TestStmtStmtParser::createParentClause(
//            TestStmtStmtParser::DECLARED_SYN, TestStmtStmtParser::CONSTANT1);
//        tup = make_tuple("Parent", "s", "1");
//        p = StmtStmtParser(tup, declList, deHelper, clsHelper);
//        actual = p.parse();
//
//        REQUIRE(actual->equals(*expected));
//        delete expected;
//        delete actual;
//    }
//
//    SECTION("test synonym/synonym") {
//        declList.push_back(&TestStmtStmtParser::DECLARED_SYN1);
//        Clause* expected = TestStmtStmtParser::createParentClause(
//            TestStmtStmtParser::DECLARED_SYN, TestStmtStmtParser::DECLARED_SYN1);
//        ClsTuple tup = make_tuple("Parent", "s", "a");
//        StmtStmtParser p(tup, declList, deHelper, clsHelper);
//        Clause* actual = p.parse();
//
//        REQUIRE(expected->equals(*actual));
//        delete expected;
//        delete actual;
//    }
//
//    SECTION("test undeclared synonym - FAIL") {
//        Clause* expected = TestStmtStmtParser::createParentClause(
//            TestStmtStmtParser::DECLARED_SYN, TestStmtStmtParser::DECLARED_SYN1);
//        ClsTuple tup = make_tuple("Parent", "s", "a");
//        StmtStmtParser p(tup, declList, deHelper, clsHelper);
//
//        REQUIRE_THROWS_AS(p.parse(), ValidityError);
//    }
//};
