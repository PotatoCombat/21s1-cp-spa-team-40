#include "catch.hpp"

#include "query_processor/QueryTokenizer.h"

struct TestQueryTokenizer {
    static bool comparePairs(DeclPair x1, DeclPair x2);
    static bool compareTuples(ClsTuple x1, ClsTuple x2);
    static bool compareVectors(vector<string> x1, vector<string> x2);
};

bool TestQueryTokenizer::comparePairs(DeclPair x1, DeclPair x2) {
    return x1.first == x2.first && x1.second == x2.second;
}

bool TestQueryTokenizer::compareTuples(ClsTuple x1, ClsTuple x2) {
    return get<0>(x1) == get<0>(x2) && get<1>(x1) == get<1>(x2) &&
           get<2>(x1) == get<2>(x2);
}

bool TestQueryTokenizer::compareVectors(vector<string> x1, vector<string> x2) {
    for (int i = 0; i < x1.size(); ++i) {
        if (x1[i] != x2[i]) {
            return false;
        }
    }
    return true;
}

TEST_CASE("QueryTokenizer: separateQueryString") {
    QueryTokenizer tokenizer;
    string DECL = "stmt s; assign a; print p1;";
    string SELECT = "Select s such that Follows(s, p1)";

    SECTION("test standard") {
        string QUERY =
            "stmt s; assign a; print p1; Select s such that Follows(s, p1)";
        pair<string, string> actual = tokenizer.separateQueryString(QUERY);
        REQUIRE(actual.first == DECL);
        REQUIRE(actual.second == SELECT);
    }

    SECTION("test no whitespace") {
        string QUERY =
            "stmt s; assign a; print p1;Select s such that Follows(s, p1)";
        pair<string, string> actual = tokenizer.separateQueryString(QUERY);
        REQUIRE(actual.first == DECL);
        REQUIRE(actual.second == SELECT);
    }

    SECTION("test newline") {
        string QUERY =
            "stmt s; assign a; print p1;\n\nSelect s such that Follows(s, p1)";
        pair<string, string> actual = tokenizer.separateQueryString(QUERY);
        REQUIRE(actual.first == DECL);
        REQUIRE(actual.second == SELECT);
    }
}

TEST_CASE("QueryTokenizer: tokenizeDeclarations") {
    QueryTokenizer tokenizer;
    vector<DeclPair> vec;

    SECTION("test standard") {
        string DECL = "stmt s; assign a; print p1;";
        DeclPair p1 = make_pair("stmt", "s");
        DeclPair p2 = make_pair("assign", "a");
        DeclPair p3 = make_pair("print", "p1");
        vector<DeclPair> expected{p1, p2, p3};

        tokenizer.tokenizeDeclarations(DECL, vec);
        REQUIRE(TestQueryTokenizer::comparePairs(vec[0], expected[0]));
        REQUIRE(TestQueryTokenizer::comparePairs(vec[1], expected[1]));
        REQUIRE(TestQueryTokenizer::comparePairs(vec[2], expected[2]));
    }
}

TEST_CASE("QueryTokenizer: tokenizeReturnSynonyms") {
    QueryTokenizer tokenizer;
    string CLAUSE = "with P.procName = \" a \"";

    SECTION("FAIL: test no Select") {
        string remaining;
        REQUIRE_THROWS_AS(tokenizer.tokenizeReturnSynonyms(
                              " s such that Parent(x, y)", remaining),
                          SyntaxError);
        REQUIRE_THROWS_AS(tokenizer.tokenizeReturnSynonyms(
                              "select s such that Parent(x, y)", remaining),
                          SyntaxError);
    }

    SECTION("SUCCESS: test one synonym") {
        string SELECT = "Select s with P.procName = \" a \"";
        string remaining;
        vector<string> actual =
            tokenizer.tokenizeReturnSynonyms(SELECT, remaining);

        vector<string> expected1 = {"s"};
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);
    }

    SECTION("SUCCESS: test BOOLEAN") {
        string SELECT = "Select BOOLEAN with P.procName = \" a \"";
        string remaining;
        vector<string> actual =
            tokenizer.tokenizeReturnSynonyms(SELECT, remaining);

        vector<string> expected1;
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);
    }

    SECTION("SUCCESS: test tuple") {
        string SELECT = "Select <P, A, L> with P.procName = \" a \"";
        string remaining;
        vector<string> actual =
            tokenizer.tokenizeReturnSynonyms(SELECT, remaining);

        vector<string> expected1 = {"P", "A", "L"};
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);

        SELECT = "Select < P ,     A,    L > with P.procName = \" a \"";
        actual = tokenizer.tokenizeReturnSynonyms(SELECT, remaining);

        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);
    }

    SECTION("SUCCESS: test attrRef without whitespace") {
        string SELECT = "Select P.stmt# with P.procName = \" a \"";
        string remaining;
        vector<string> actual =
            tokenizer.tokenizeReturnSynonyms(SELECT, remaining);

        vector<string> expected1 = {"P.stmt#"};
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);

        SELECT = "Select P.procName with P.procName = \" a \"";
        actual = tokenizer.tokenizeReturnSynonyms(SELECT, remaining);
        expected1 = {"P.procName"};
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);

        SELECT = "Select <P, A.procName, L.stmt#> with P.procName = \" a \"";
        actual = tokenizer.tokenizeReturnSynonyms(SELECT, remaining);
        expected1 = {"P", "A.procName", "L.stmt#"};
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);
    }

    SECTION("SUCCESS: test attrRef with whitespace") {
        string SELECT = "Select P . stmt# with P.procName = \" a \"";
        string remaining;
        vector<string> actual =
            tokenizer.tokenizeReturnSynonyms(SELECT, remaining);

        vector<string> expected1 = {"P.stmt#"};
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);

        SELECT = "Select P.    procName with P.procName = \" a \"";
        actual = tokenizer.tokenizeReturnSynonyms(SELECT, remaining);
        expected1 = {"P.procName"};
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);

        SELECT =
            "Select <P, A     .procName, L . stmt#> with P.procName = \" a \"";
        actual = tokenizer.tokenizeReturnSynonyms(SELECT, remaining);
        expected1 = {"P", "A.procName", "L.stmt#"};
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);
    }
}

TEST_CASE("QueryTokenizer: tokenizeClauses for such that") {
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

TEST_CASE("QueryTokenizer: tokenizeClauses for pattern") {
    QueryTokenizer tokenizer;
    vector<ClsTuple> rel;
    vector<PatTuple> pat;
    vector<WithTuple> wit;

    SECTION("SUCCESS: test assign wildcard, wildcard") {
        string input = "pattern a(_, _)";
        PatTuple expected = make_pair("a", vector<string>{"_", "_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(  _, _)";
        expected = make_pair("a", vector<string>{"_", "_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(_  , _)";
        expected = make_pair("a", vector<string>{"_", "_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(_,_)";
        expected = make_pair("a", vector<string>{"_", "_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(_, _   )";
        expected = make_pair("a", vector<string>{"_", "_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));
    }

    SECTION("SUCCESS: test assign wildcard, ?") {
        string input = "pattern a(_, _\"x\"_)"; // (_, _"x"_)
        PatTuple expected = make_pair("a", vector<string>{"_", "_\"x\"_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(_, _\" x + 5 \"_)"; // (_, _" x + 5 "_)
        expected = make_pair("a", vector<string>{"_", "_\"x + 5\"_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(_, \"x + 5\")"; // (_, "x + 5")
        expected = make_pair("a", vector<string>{"_", "\"x + 5\""});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(_, \" x + 5 \"  )"; // (_, " x + 5 ")
        expected = make_pair("a", vector<string>{"_", "\"x + 5\""});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));
    }

    SECTION("SUCCESS: test assign syn, ?") {
        string input = "pattern a(synonym, _\" x \"_)"; // (synonym, _" x "_)
        PatTuple expected =
            make_pair("a", vector<string>{"synonym", "_\"x\"_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(    synonym, _\" x \"_)"; // (synonym, _" x "_)
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(synonym    , _\" x \"_)"; // (synonym, _" x "_)
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));
    }

    SECTION("SUCCESS: test assign quoted, ?") {
        string input = "pattern a(\"quoted\", \" x * y \")"; // ("quoted", "x * y")
        PatTuple expected =
            make_pair("a", vector<string>{"\"quoted\"", "\"x * y\""});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(  \"quoted\"  , \" x * y \")"; // ("quoted", "x * y")
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));

        pat.clear();
        input = "pattern a(\"    quoted \",\" x * y \")"; // ("quoted", "x * y")
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(pat[0].first == expected.first);
        REQUIRE(
            TestQueryTokenizer::compareVectors(pat[0].second, expected.second));
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
//}
