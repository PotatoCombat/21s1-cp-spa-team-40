#include "catch.hpp"

#include "query_processor/QueryTokenizer.h"

struct TestQueryTokenizer {
    static const string QUERY1;
    static const string DECL1;
    static const string CLAUSE1;
    static const string QUERY_NO_WHITESPACE;
    static const string QUERY_NEWLINE;
    static bool compareTuples(ClsTuple r1, ClsTuple r2);
    static vector<DeclPair> createDecls();
};

const string TestQueryTokenizer::QUERY1 =
    "stmt s; assign a; print p1; Select s such that Follows(s, p1)";

const string TestQueryTokenizer::DECL1 = "stmt s; assign a; print p1;";

const string TestQueryTokenizer::CLAUSE1 = "Select s such that Follows(s, p1)";

const string TestQueryTokenizer::QUERY_NO_WHITESPACE =
    "stmt s; assign a; print p1;Select s such that Follows(s, p1)";

const string TestQueryTokenizer::QUERY_NEWLINE =
    "stmt s; assign a; print p1;\n\nSelect s such that Follows(s, p1)";

bool TestQueryTokenizer::compareTuples(ClsTuple r1, ClsTuple r2) {
    return get<0>(r1) == get<0>(r2) && get<1>(r1) == get<1>(r2) &&
           get<2>(r1) == get<2>(r2);
}

vector<DeclPair> TestQueryTokenizer::createDecls() {
    DeclPair p1 = make_pair("stmt", "s");
    DeclPair p2 = make_pair("assign", "a");
    DeclPair p3 = make_pair("print", "p1");
    return vector<DeclPair>{p1, p2, p3};
}

TEST_CASE("QueryTokenizer: separateDeclaration") {
    QueryTokenizer tokenizer;
    string expectedD = TestQueryTokenizer::DECL1;
    string expectedC = TestQueryTokenizer::CLAUSE1;

    SECTION("test standard") {
        pair<string, string> actual =
            tokenizer.separateQueryString(TestQueryTokenizer::QUERY1);
        REQUIRE(actual.first == expectedD);
        REQUIRE(actual.second == expectedC);
    }

    SECTION("test no whitespace") {
        pair<string, string> actual = tokenizer.separateQueryString(
            TestQueryTokenizer::QUERY_NO_WHITESPACE);
        REQUIRE(actual.first == expectedD);
        REQUIRE(actual.second == expectedC);
    }

    SECTION("test newline") {
        pair<string, string> actual =
            tokenizer.separateQueryString(TestQueryTokenizer::QUERY_NEWLINE);
        REQUIRE(actual.first == expectedD);
        REQUIRE(actual.second == expectedC);
    }
}

TEST_CASE("QueryTokenizer: tokenizeDeclaration") {
    QueryTokenizer tokenizer;
    vector<DeclPair> vec;
    vector<DeclPair> expected = TestQueryTokenizer::createDecls();

    SECTION("test standard") {
        tokenizer.tokenizeDeclarations(TestQueryTokenizer::DECL1, vec);
        REQUIRE((get<0>(vec[0]) == get<0>(expected[0]) &&
                 get<1>(vec[0]) == get<1>(expected[0])));
        REQUIRE((get<0>(vec[1]) == get<0>(expected[1]) &&
                 get<1>(vec[1]) == get<1>(expected[1])));
        REQUIRE((get<0>(vec[2]) == get<0>(expected[2]) &&
                 get<1>(vec[2]) == get<1>(expected[2])));
    }

    SECTION("test invalid name") {
        string invalid = "stmt 0PD;";
        REQUIRE_THROWS_AS(tokenizer.tokenizeDeclarations(invalid, vec),
                          SyntaxError);

        invalid = "assign s_fishy;";
        REQUIRE_THROWS_AS(tokenizer.tokenizeDeclarations(invalid, vec),
                          SyntaxError);

        invalid = "print 11111;";
        REQUIRE_THROWS_AS(tokenizer.tokenizeDeclarations(invalid, vec),
                          SyntaxError);
    }
}

TEST_CASE("QueryTokenizer: tokenizeReturn") {
    QueryTokenizer tokenizer;

    SECTION("test standard") {
        string actual;
        string remaining;
        tokenizer.tokenizeReturnSynonym(TestQueryTokenizer::CLAUSE1, actual,
                                        remaining);
        string expected1 = "s";
        string expected2 = "such that Follows(s, p1)";
        REQUIRE(actual == expected1);
        REQUIRE(remaining == expected2);
    }

    SECTION("test no Select") {
        string actual;
        string remaining;
        REQUIRE_THROWS(tokenizer.tokenizeReturnSynonym(
            " s such that Parent(x, y)", actual, remaining));
        REQUIRE_THROWS(tokenizer.tokenizeReturnSynonym(
            "select s such that Parent(x, y)", actual, remaining));
    }
}

TEST_CASE("QueryTokenizer: tokenizeClause") {
    QueryTokenizer tokenizer;

    SECTION("test only one such that clause") {
        string input = "such that Follows(s, p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithTuple> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(get<0>(rel[0]) == get<0>(expected[0]));
        REQUIRE(get<1>(rel[0]) == get<1>(expected[0]));
        REQUIRE(get<2>(rel[0]) == get<2>(expected[0]));
        REQUIRE(pat.size() == 0);
    }

    SECTION("test extra whitespace 1") {
        string input = "such that    Follows(s, p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithTuple> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(get<0>(rel[0]) == get<0>(expected[0]));
        REQUIRE(get<1>(rel[0]) == get<1>(expected[0]));
        REQUIRE(get<2>(rel[0]) == get<2>(expected[0]));
        REQUIRE(pat.size() == 0);
    }

    SECTION("test extra whitespace 2") {
        string input = "such      that Follows(s, p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithTuple> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(get<0>(rel[0]) == get<0>(expected[0]));
        REQUIRE(get<1>(rel[0]) == get<1>(expected[0]));
        REQUIRE(get<2>(rel[0]) == get<2>(expected[0]));
        REQUIRE(pat.size() == 0);
    }

    SECTION("test extra whitespace 3") {
        string input = "such that Follows (s, p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithTuple> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(get<0>(rel[0]) == get<0>(expected[0]));
        REQUIRE(get<1>(rel[0]) == get<1>(expected[0]));
        REQUIRE(get<2>(rel[0]) == get<2>(expected[0]));
        REQUIRE(pat.size() == 0);
    }

    SECTION("test extra whitespace 4") {
        string input = "such that Follows( s,p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithTuple> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(get<0>(rel[0]) == get<0>(expected[0]));
        REQUIRE(get<1>(rel[0]) == get<1>(expected[0]));
        REQUIRE(get<2>(rel[0]) == get<2>(expected[0]));
        REQUIRE(pat.size() == 0);
    }

    SECTION("test extra whitespace 5") {
        string input = "such that Follows(s , p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithTuple> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(get<0>(rel[0]) == get<0>(expected[0]));
        REQUIRE(get<1>(rel[0]) == get<1>(expected[0]));
        REQUIRE(get<2>(rel[0]) == get<2>(expected[0]));
        REQUIRE(pat.size() == 0);
    }

    SECTION("test extra whitespace 6") {
        string input = "such that Follows(s ,p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithTuple> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(get<0>(rel[0]) == get<0>(expected[0]));
        REQUIRE(get<1>(rel[0]) == get<1>(expected[0]));
        REQUIRE(get<2>(rel[0]) == get<2>(expected[0]));
        REQUIRE(pat.size() == 0);
    }

    SECTION("test extra whitespace 7") {
        string input = "such that Follows(s, p1 )";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithTuple> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(get<0>(rel[0]) == get<0>(expected[0]));
        REQUIRE(get<1>(rel[0]) == get<1>(expected[0]));
        REQUIRE(get<2>(rel[0]) == get<2>(expected[0]));
        REQUIRE(pat.size() == 0);
    }

    SECTION("test no whitespace") {
        string input = "such that Follows(s,p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithTuple> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(get<0>(rel[0]) == get<0>(expected[0]));
        REQUIRE(get<1>(rel[0]) == get<1>(expected[0]));
        REQUIRE(get<2>(rel[0]) == get<2>(expected[0]));
        REQUIRE(pat.size() == 0);
    }
}

TEST_CASE("QueryTokenizer: tokenizeClause for pattern") {
    QueryTokenizer tokenizer;

    SECTION("test partial match pattern") {
        string input = "pattern a(_, _\" x \"_)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithTuple> wit;
        vector<PatTuple> expected = vector<PatTuple>{make_tuple("a", "_", "x")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.size() == 0);
        REQUIRE(get<0>(pat[0]) == get<0>(expected[0]));
        REQUIRE(get<1>(pat[0]) == get<1>(expected[0]));
        REQUIRE(get<2>(pat[0]) == get<2>(expected[0]));
    }
}

// TEST_CASE("QueryTokenizer: trim") {
//    QueryTokenizer tokenizer;
//    string input = "    \t\n\n       some thing\n   ";
//
//    SECTION("trim") {
//        string expected = "some thing";
//        string actual = tokenizer.trim(input);
//        REQUIRE(expected == actual);
//        REQUIRE(actual != input);
//
//        string empty = "       ";
//        REQUIRE(tokenizer.trim(empty) == "");
//    }
//
//    SECTION("trimL") {
//        string expected = "some thing\n   ";
//        string actual = tokenizer.trimL(input);
//        REQUIRE(expected == actual);
//
//        string empty = "       ";
//        REQUIRE(tokenizer.trim(empty) == "");
//    }
//
//    SECTION("trimR") {
//        string expected = "    \t\n\n       some thing";
//        string actual = tokenizer.trimR(input);
//        REQUIRE(expected == actual);
//
//        string empty = "       ";
//        REQUIRE(tokenizer.trim(empty) == "");
//    }
//}

// TEST_CASE("QueryTokenizer: splitComma") {
//    QueryTokenizer tokenizer;
//    SECTION("test standard") {
//        vector<string> result{ "x", "y", "z" };
//        vector<string> actual;
//        string input = "x, y, z";
//        tokenizer.splitComma(input, actual);
//        REQUIRE(actual == result);
//    }
//
//    SECTION("test space") {
//        vector<string> result{ "x", "y", "z" };
//        vector<string> actual;
//        string input = "x,   y,   z";
//        tokenizer.splitComma(input, actual);
//        REQUIRE(actual == result);
//    }
//
//    SECTION("test no comma") {
//        string input = "xyz";
//        vector<string> result1{ "xyz" };
//        vector<string> actual;
//        tokenizer.splitComma(input, actual);
//        REQUIRE(actual == result1);
//    }
//}

// TEST_CASE("QueryTokenizer: splitBCBRel") {
//    QueryTokenizer tokenizer;
//    ClsTuple tup;
//    ClsTuple correct = make_tuple("Follows", "name", "nextline");
//    string in1 = "Follows(name, nextline)";
//    string in2 = "Follows ( name , nextline )";
//    string in3 = "Follows(, )";
//    string in4 = "Follows(name nextline)";
//    string in5 = "Follows name, nextline)";
//    string in6 = "Follows(name,, nextline)";
//    string in7 = "Follows(name, nextline))";
//    string in8 = "Follows((name, nextline)";
//    string in9 = "Follows, name( nextline)";
//    string in10 = "Follows(,nextline)";
//    string in11 = "Follows(name,)";
//    string in12 = "(name, nextline)";
//
//    SECTION("test passes") {
//        tokenizer.splitBCBRel(in1, tup);
//        REQUIRE(TestQueryTokenizer::compareTuples(tup, correct));
//        tokenizer.splitBCBRel(in2, tup);
//        REQUIRE(TestQueryTokenizer::compareTuples(tup, correct));
//    }
//
//    SECTION("test fail throws error") {
//        REQUIRE_THROWS_WITH(tokenizer.splitBCBRel(in3, tup), "error");
//        REQUIRE_THROWS_WITH(tokenizer.splitBCBRel(in4, tup), "error");
//        REQUIRE_THROWS_WITH(tokenizer.splitBCBRel(in5, tup), "error");
//        REQUIRE_THROWS_WITH(tokenizer.splitBCBRel(in6, tup), "error");
//        REQUIRE_THROWS_WITH(tokenizer.splitBCBRel(in7, tup), "error");
//        REQUIRE_THROWS_WITH(tokenizer.splitBCBRel(in8, tup), "error");
//        REQUIRE_THROWS_WITH(tokenizer.splitBCBRel(in9, tup), "error");
//        REQUIRE_THROWS_WITH(tokenizer.splitBCBRel(in10, tup), "error");
//        REQUIRE_THROWS_WITH(tokenizer.splitBCBRel(in11, tup), "error");
//        REQUIRE_THROWS_WITH(tokenizer.splitBCBRel(in12, tup), "error");
//    }
//}
