#include "catch.hpp"

#include "query_processor/QueryTokenizer.h"

struct TestQueryTokenizer {
    static bool comparePairs(DeclPair x1, DeclPair x2);
    static bool compareTuples(ClsTuple x1, ClsTuple x2);
    static bool compareVectors(vector<string> x1, vector<string> x2);
    static bool compareVectorPatTuple(vector<PatTuple> x1, vector<PatTuple> x2);
    static bool compareVectorPair(vector<ClsTuple> x1, vector<ClsTuple> x2);
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

bool TestQueryTokenizer::compareVectorPatTuple(vector<PatTuple> x1,
                                               vector<PatTuple> x2) {
    for (int i = 0; i < x1.size(); ++i) {
        if (get<0>(x1[i]) != get<0>(x2[i]) || get<1>(x1[i]) != get<1>(x2[i]) ||
            !compareVectors(get<2>(x1[i]), get<2>(x2[i]))) {
            return false;
        }
    }
    return true;
}

bool TestQueryTokenizer::compareVectorPair(vector<ClsTuple> x1,
                                           vector<ClsTuple> x2) {
    for (int i = 0; i < x1.size(); ++i) {
        if (!compareTuples(x1[i], x2[i])) {
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

    SECTION("PASS: test one synonym") {
        string SELECT = "Select s with P.procName = \" a \"";
        string remaining;
        vector<string> actual =
            tokenizer.tokenizeReturnSynonyms(SELECT, remaining);

        vector<string> expected1 = {"s"};
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);
    }

    SECTION("PASS: test BOOLEAN") {
        string SELECT = "Select BOOLEAN with P.procName = \" a \"";
        string remaining;
        vector<string> actual =
            tokenizer.tokenizeReturnSynonyms(SELECT, remaining);

        vector<string> expected1;
        REQUIRE(TestQueryTokenizer::compareVectors(actual, expected1));
        REQUIRE(remaining == CLAUSE);
    }

    SECTION("PASS: test tuple") {
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

    SECTION("PASS: test attrRef without whitespace") {
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

    SECTION("PASS: test attrRef with whitespace") {
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

    SECTION("PASS: test assign wildcard, wildcard") {
        string input = "pattern a(_, _)";
        PatTuple expected = make_tuple("a", "_", vector<string>{"_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(get<0>(pat[0]) == get<0>(expected));
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(  _, _)";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(get<0>(pat[0]) == get<0>(expected));
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(_  , _)";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(get<0>(pat[0]) == get<0>(expected));
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(_,_)";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(get<0>(pat[0]) == get<0>(expected));
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(_, _   )";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(get<0>(pat[0]) == get<0>(expected));
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));
    }

    SECTION("PASS: test assign wildcard, ?") {
        string input = "pattern a(_, _\"x\"_)"; // (_, _"x"_)
        PatTuple expected =
            make_tuple("a", "_", vector<string>{"_\"x\"_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(_, _\" x + 5 \"_)"; // (_, _" x + 5 "_)
        expected = make_tuple("a", "_", vector<string>{"_\"x + 5\"_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(_, \"x + 5\")"; // (_, "x + 5")
        expected = make_tuple("a", "_", vector<string>{"\"x + 5\""});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(_, \" x + 5 \"  )"; // (_, " x + 5 ")
        expected = make_tuple("a", "_", vector<string>{"\"x + 5\""});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));
    }

    SECTION("PASS: test assign syn, ?") {
        string input = "pattern a(synonym, _\" x \"_)"; // (synonym, _" x "_)
        PatTuple expected =
            make_tuple("a", "synonym", vector<string>{"_\"x\"_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(    synonym, _\" x \"_)"; // (synonym, _" x "_)
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(synonym    , _\" x \"_)"; // (synonym, _" x "_)
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));
    }

    SECTION("PASS: test assign quoted, ?") {
        string input =
            "pattern a(\"quoted\", \" x * y \")"; // ("quoted", "x * y")
        PatTuple expected =
            make_tuple("a", "\"quoted\"", vector<string>{"\"x * y\""});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(  \"quoted\"  , \" x * y \")"; // ("quoted", "x * y")
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input =
            "pattern a(\"    quoted \",\" (x * y) \")"; // ("quoted", "(x * y)")
        expected = make_tuple("a", "\"quoted\"", vector<string>{"\"(x * y)\""});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));
    }

    SECTION("PASS: test if _, _, _") {
        string input = "pattern ifs(_, _, _)";
        PatTuple expected = make_tuple("ifs", "_", vector<string>{"_", "_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern ifs(_ , _ , _)";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern ifs( _,    _ , _   )";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));
    }

    SECTION("PASS: test if syn, _, _") {
        string input = "pattern ifs(synonym, _, _)";
        PatTuple expected =
            make_tuple("ifs", "synonym", vector<string>{"_", "_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern ifs(synonym , _ , _)";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern ifs( synonym,    _ , _   )";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));
    }

    SECTION("PASS: test if quoted, _, _") {
        string input = "pattern ifs(\" quoted \", _, _)";
        PatTuple expected =
            make_tuple("ifs", "\"quoted\"", vector<string>{"_", "_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern ifs(\" quoted \" , _ , _)";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern ifs( \"quoted\",    _ , _   )";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));
    }
}

TEST_CASE("QueryTokenizer: tokenizeClauses for multiple pattern clauses") {
    QueryTokenizer tokenizer;
    vector<ClsTuple> rel;
    vector<PatTuple> pat;
    vector<WithTuple> wit;

    SECTION("PASS: test pattern x(x,x) pattern x(x,x)") {
        string input = "pattern a(_, _) pattern w (_, _)";
        vector<PatTuple> expected;
        expected.push_back(make_tuple("a", "_", vector<string>{"_"}));
        expected.push_back(make_tuple("w", "_", vector<string>{"_"}));
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(pat, expected));

        pat.clear();
        expected.clear();
        input = "pattern a(x, \" x - (y + 5) \") pattern w(\"x\", _)";
        expected.push_back(
            make_tuple("a", "x", vector<string>{"\"x - (y + 5)\""}));
        expected.push_back(make_tuple("w", "\"x\"", vector<string>{"_"}));
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(pat, expected));

        pat.clear();
        expected.clear();
        input = "pattern a(x, \" x - (y + 5) \") and a(\"x\", _\" x + y * (x + "
                "y)\"_)";
        expected.push_back(
            make_tuple("a", "x", vector<string>{"\"x - (y + 5)\""}));
        expected.push_back(
            make_tuple("a", "\"x\"", vector<string>{"_\"x + y * (x + y)\"_"}));
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(pat, expected));
    }

    SECTION("PASS: test pattern x(x,x)/pattern x(x,x,x)") {
        string input = "pattern a(_, _) pattern ifs (x, _, _)";
        vector<PatTuple> expected;
        expected.push_back(make_tuple("a", "_", vector<string>{"_"}));
        expected.push_back(make_tuple("ifs", "x", vector<string>{"_", "_"}));
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(pat, expected));

        pat.clear();
        expected.clear();
        input = "pattern ifs(x, _, _) and a(x, \" x - (y + 5) \") pattern "
                "a(\"x\", \" x+y * ( x+y )\")";
        expected.push_back(make_tuple("ifs", "x", vector<string>{"_", "_"}));
        expected.push_back(
            make_tuple("a", "x", vector<string>{"\"x - (y + 5)\""}));
        expected.push_back(
            make_tuple("a", "\"x\"", vector<string>{"\"x+y * ( x+y )\""}));
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(pat.size() == expected.size());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(pat, expected));
    }
}

TEST_CASE("QueryTokenizer: tokenizeClauses N clauses (pat + cls + and)") {
    QueryTokenizer tokenizer;
    vector<ClsTuple> rel;
    vector<PatTuple> pat;
    vector<WithTuple> wit;

    SECTION("PASS: pattern and such that such that pattern") {
        string input = "pattern a(x, _) and i(_, _, _ ) such that Calls*(p1, "
                       "p2) such that Parent(i2, i) pattern i2(x, _, _)";
        vector<PatTuple> expP;
        vector<ClsTuple> relP;
        expP.push_back(make_tuple("a", "x", vector<string>{"_"}));
        expP.push_back(make_tuple("i", "_", vector<string>{"_", "_"}));
        relP.push_back(make_tuple("Calls*", "p1", "p2"));
        relP.push_back(make_tuple("Parent", "i2", "i"));
        expP.push_back(make_tuple("i2", "x", vector<string>{"_", "_"}));
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(pat, expP));
        REQUIRE(TestQueryTokenizer::compareVectorPair(rel, relP));
    }

    SECTION("PASS: such that and and such that and") {
        string input =
            "such that Calls*(p1, p2) and Parent(a1, c2) and Parent*(a1, c2) "
            "such that Follows*(i2, i) and Uses(p1, x)";
        vector<ClsTuple> relP;
        relP.push_back(make_tuple("Calls*", "p1", "p2"));
        relP.push_back(make_tuple("Parent", "a1", "c2"));
        relP.push_back(make_tuple("Parent*", "a1", "c2"));
        relP.push_back(make_tuple("Follows*", "i2", "i"));
        relP.push_back(make_tuple("Uses", "p1", "x"));
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(rel, relP));
    }

    SECTION("FAIL: start with `and`") {
        string input = "and a(x, _) such that Calls*(p1, p2)";
        REQUIRE_THROWS_AS(tokenizer.tokenizeClauses(input, rel, pat, wit),
                          SyntaxError);
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
