#include "catch.hpp"
#include <string>

#include "query_processor/query_preprocessor/QueryTokenizer.h"

struct TestQueryTokenizer {
    static const string QUERY1;
    static const string DECL1;
    static const string CLAUSE1;
    static const string QUERY_NO_WHITESPACE;
    static const string QUERY_NEWLINE;
    static pair<string, string> splitIntoParts();
    static vector<pair<string, string>> tokenizeDeclaration();
    static string tokenizeReturnEntity();
    static vector<tuple<string, string, string>> tokenizeSuchThatClause();
    static vector<tuple<string, string, string>> tokenizePatternClause();
};

const string TestQueryTokenizer::QUERY1 = 
    "stmt s; assign a; print p1; Select s such that Follows(s, p1)";

const string TestQueryTokenizer::DECL1 =
    "stmt s; assign a; print p1;";

const string TestQueryTokenizer::CLAUSE1 =
    "Select s such that Follows(s, p1)";

const string TestQueryTokenizer::QUERY_NO_WHITESPACE =
    "stmt s; assign a; print p1;Select s such that Follows(s, p1)";

const string TestQueryTokenizer::QUERY_NEWLINE =
    "stmt s; assign a; print p1;\n\nSelect s such that Follows(s, p1)";

pair<string, string> TestQueryTokenizer::splitIntoParts() {
    return make_pair(DECL1, CLAUSE1);
}

vector<pair<string, string>> TestQueryTokenizer::tokenizeDeclaration() {
    pair<string, string> p1 = make_pair("stmt", "s");
    pair<string, string> p2 = make_pair("assign", "a");
    pair<string, string> p3 = make_pair("print", "p");

    return vector<pair<string, string>>{p1, p2, p3};
}

string TestQueryTokenizer::tokenizeReturnEntity() {
    return "s";
}

vector<tuple<string, string, string>> TestQueryTokenizer::tokenizeSuchThatClause() {
    tuple<string, string, string> t = make_tuple("Follows", "s", "p1");

    return vector<tuple<string, string, string>>{t};
}

vector<tuple<string, string, string>> TestQueryTokenizer::tokenizePatternClause() {
    return vector<tuple<string, string, string>>();
}


TEST_CASE("QP-QueryTokenizer: splitIntoParts") {
    QueryTokenizer tokenizer;

    SECTION("test standard") {
        pair<string, string> expected = TestQueryTokenizer::splitIntoParts();
        pair<string, string> actual = tokenizer.splitIntoParts(TestQueryTokenizer::QUERY1);
        REQUIRE(expected.first == actual.first);
        REQUIRE(expected.second == actual.second);
    }

    SECTION("test no whitespace") {
        pair<string, string> expected = TestQueryTokenizer::splitIntoParts();
        pair<string, string> actual = tokenizer.splitIntoParts(TestQueryTokenizer::QUERY_NO_WHITESPACE);
        REQUIRE(expected.first == actual.first);
        REQUIRE(expected.second == actual.second);
    }

    SECTION("test newline") {
        pair<string, string> expected = TestQueryTokenizer::splitIntoParts();
        pair<string, string> actual = tokenizer.splitIntoParts(TestQueryTokenizer::QUERY_NEWLINE);
        REQUIRE(expected.first == actual.first);
        REQUIRE(expected.second == actual.second);
    }

    //REQUIRE_THROWS_WITH(tokenizer.splitIntoParts(decl), "Empty select clause");
    //REQUIRE_THROWS_WITH(tokenizer.splitIntoParts(selectcl), "Empty declaration");
}

TEST_CASE("QP-QueryTokenizer: tokenizeDeclaration") {
    QueryTokenizer tokenizer;

    SECTION("test standard") {
        vector<pair<string, string>> actual = tokenizer.tokenizeDeclaration(TestQueryTokenizer::DECL1);
        vector<pair<string, string>> expected = TestQueryTokenizer::tokenizeDeclaration();
        REQUIRE(get<0>(expected[0]) == get<0>(actual[0]));
        REQUIRE(get<1>(expected[0]) == get<1>(actual[0]));
        REQUIRE(get<0>(expected[1]) == get<0>(actual[1]));
        REQUIRE(get<1>(expected[1]) == get<1>(actual[1]));
        REQUIRE(get<0>(expected[2]) == get<0>(actual[2]));
        REQUIRE(get<1>(expected[2]) == get<1>(actual[2]));
    }

}

//TEST_CASE("QP-QueryTokenizer: tokenizeSelectClause") {
//    string expectedReturn;
//    pair<string, vector<string>> selectClause;
//    string expectedClause;
//
//    expectedReturn = "s";
//    selectClause = tokenizer.tokenizeClauses(selectcl);
//    REQUIRE(selectClause.first == expectedReturn);
//    REQUIRE(selectClause.second == vector<string>());
//    
//    expectedClause = "Follows(s, p1)";
//    selectClause = tokenizer.tokenizeClauses(selectcl1);
//    REQUIRE(selectClause.first == expectedReturn);
//    REQUIRE(selectClause.second[0] == expectedClause);
//}
