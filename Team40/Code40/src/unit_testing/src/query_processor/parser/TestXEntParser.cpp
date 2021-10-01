
//TEST_CASE("XEntParser: first argument cannot be wildcard") {
//    vector<Reference *> declList;
//    DesignEntityTypeHelper deHelper;
//    ClauseTypeHelper clsHelper;
//
//    ClsTuple tup = make_tuple("Modifies", "_", "v");
//    XEntParser p(tup, declList, deHelper, clsHelper);
//
//    REQUIRE_THROWS_AS(p.parse(), ValidityError);
//}
//
//TEST_CASE(
//    "XEntParser: differentiate between a statement or procedure constant") {
//    vector<Reference *> declList;
//    declList.push_back(&TestXEntParser::STMT);
//    declList.push_back(&TestXEntParser::PROCEDURE);
//    declList.push_back(&TestXEntParser::VARIABLE);
//    DesignEntityTypeHelper deHelper;
//    ClauseTypeHelper clsHelper;
//
//    SECTION("test quoted should become ModifiesP") {
//        Clause *expected = TestXEntParser::createModifiesPClause(
//            TestXEntParser::CONSTANT_PROCEDURE, TestXEntParser::VARIABLE);
//        ClsTuple tup = make_tuple("Modifies", "\"procedure\"", "v");
//        XEntParser p(tup, declList, deHelper, clsHelper);
//        Clause *actual = p.parse();
//
//        REQUIRE(actual->equals(*expected));
//        delete expected;
//        delete actual;
//    }
//
//    SECTION("test integer should become ModifiesS") {
//        Clause *expected = TestXEntParser::createModifiesSClause(
//            TestXEntParser::CONSTANT_STMT, TestXEntParser::VARIABLE);
//        ClsTuple tup = make_tuple("Modifies", "1", "v");
//        XEntParser p(tup, declList, deHelper, clsHelper);
//        Clause *actual = p.parse();
//
//        REQUIRE(actual->equals(*expected));
//        delete expected;
//        delete actual;
//    }
//}
//
//TEST_CASE("XEntParser: second argument wildcard") {
//    vector<Reference *> declList;
//    declList.push_back(&TestXEntParser::STMT);
//    DesignEntityTypeHelper deHelper;
//    ClauseTypeHelper clsHelper;
//
//    Clause *expected = TestXEntParser::createModifiesSClause(
//        TestXEntParser::STMT, TestXEntParser::WILDCARD);
//    ClsTuple tup = make_tuple("Modifies", "s", "_");
//    XEntParser p(tup, declList, deHelper, clsHelper);
//    Clause *actual = p.parse();
//
//    REQUIRE(actual->equals(*expected));
//    delete expected;
//    delete actual;
//}
//
//TEST_CASE("XEntParser: second argument constant") {
//    vector<Reference*> declList;
//    declList.push_back(&TestXEntParser::STMT);
//    DesignEntityTypeHelper deHelper;
//    ClauseTypeHelper clsHelper;
//
//    Clause* expected = TestXEntParser::createModifiesSClause(
//        TestXEntParser::STMT, TestXEntParser::CONSTANT_VARIABLE);
//    ClsTuple tup = make_tuple("Modifies", "s", "\"variable\"");
//    XEntParser p(tup, declList, deHelper, clsHelper);
//    Clause* actual = p.parse();
//
//    REQUIRE(actual->equals(*expected));
//    delete expected;
//    delete actual;
//}
//
//TEST_CASE("XEntParer: undeclared synonym") {
//    vector<Reference*> declList;
//    declList.push_back(&TestXEntParser::STMT);
//    declList.push_back(&TestXEntParser::PROCEDURE);
//    declList.push_back(&TestXEntParser::VARIABLE);
//    DesignEntityTypeHelper deHelper;
//    ClauseTypeHelper clsHelper;
//
//    SECTION("test undeclared first declared second") {
//        ClsTuple tup = make_tuple("Modifies", "a", "v");
//        XEntParser p(tup, declList, deHelper, clsHelper);
//        REQUIRE_THROWS_AS(p.parse(), ValidityError);
//    }
//
//    SECTION("test declared first undeclared second") {
//        ClsTuple tup = make_tuple("Modifies", "s", "somevariable");
//        XEntParser p(tup, declList, deHelper, clsHelper);
//        REQUIRE_THROWS_AS(p.parse(), ValidityError);
//    }
//}
