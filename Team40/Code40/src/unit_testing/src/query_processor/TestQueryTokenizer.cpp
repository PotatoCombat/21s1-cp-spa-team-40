#include "catch.hpp"

#include "query_processor/QueryTokenizer.h"

struct TestQueryTokenizer {
    static bool comparePairs(DeclPair x1, DeclPair x2);
    static bool compareTuples(ClsTuple x1, ClsTuple x2);
    static bool compareVectors(vector<string> x1, vector<string> x2);
    static bool compareVectorPatTuple(vector<PatTuple> x1, vector<PatTuple> x2);
    static bool compareVectorTuple(vector<ClsTuple> x1, vector<ClsTuple> x2);
    static bool compareVectorPair(vector<DeclPair> x1, vector<DeclPair> x2);
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

bool TestQueryTokenizer::compareVectorTuple(vector<ClsTuple> x1,
                                            vector<ClsTuple> x2) {
    for (int i = 0; i < x1.size(); ++i) {
        if (!compareTuples(x1[i], x2[i])) {
            return false;
        }
    }
    return true;
}

bool TestQueryTokenizer::compareVectorPair(vector<DeclPair> x1,
                                           vector<DeclPair> x2) {
    for (int i = 0; i < x1.size(); ++i) {
        if (!comparePairs(x1[i], x2[i])) {
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
        REQUIRE(TestQueryTokenizer::compareVectorPair(vec, expected));
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
        vector<WithPair> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareTuples(rel[0], expected[0]));
    }

    SECTION("test extra whitespace 1") {
        string input = "such that    Follows(s, p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareTuples(rel[0], expected[0]));
    }

    SECTION("test extra whitespace 2") {
        string input = "such      that Follows(s, p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareTuples(rel[0], expected[0]));
    }

    SECTION("test extra whitespace 3") {
        string input = "such that Follows (s, p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareTuples(rel[0], expected[0]));
    }

    SECTION("test extra whitespace 4") {
        string input = "such that Follows( s,p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareTuples(rel[0], expected[0]));
    }

    SECTION("test extra whitespace 5") {
        string input = "such that Follows(s , p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareTuples(rel[0], expected[0]));
    }

    SECTION("test extra whitespace 6") {
        string input = "such that Follows(s ,p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareTuples(rel[0], expected[0]));
    }

    SECTION("test extra whitespace 7") {
        string input = "such that Follows(s, p1 )";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareTuples(rel[0], expected[0]));
    }

    SECTION("test no whitespace") {
        string input = "such that Follows(s,p1)";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<ClsTuple> expected =
            vector<ClsTuple>{make_tuple("Follows", "s", "p1")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareTuples(rel[0], expected[0]));
    }
}

TEST_CASE("QueryTokenizer: tokenizeClauses for pattern") {
    QueryTokenizer tokenizer;
    vector<ClsTuple> rel;
    vector<PatTuple> pat;
    vector<WithPair> wit;

    SECTION("PASS: test assign wildcard, wildcard") {
        string input = "pattern a(_, _)";
        PatTuple expected = make_tuple("a", "_", vector<string>{"_"});
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(  _, _)";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(_  , _)";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(_,_)";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));

        pat.clear();
        input = "pattern a(_, _   )";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        CHECK(rel.empty());
        CHECK(wit.empty());
        REQUIRE(TestQueryTokenizer::compareVectorPatTuple(
            pat, vector<PatTuple>{expected}));
    }

    SECTION("PASS: test assign wildcard, ?") {
        string input = "pattern a(_, _\"x\"_)"; // (_, _"x"_)
        PatTuple expected = make_tuple("a", "_", vector<string>{"_\"x\"_"});
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

    SECTION("PASS: test assign empty pattern string") {
        string input = "pattern a(_, \" \")";
        PatTuple expected = make_tuple("a", "_", vector<string>{"\"\""});
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
    vector<WithPair> wit;

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
    vector<WithPair> wit;

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
        REQUIRE(TestQueryTokenizer::compareVectorTuple(rel, relP));
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
        REQUIRE(TestQueryTokenizer::compareVectorTuple(rel, relP));
    }

    SECTION("FAIL: start with `and`") {
        string input = "and a(x, _) such that Calls*(p1, p2)";
        REQUIRE_THROWS_AS(tokenizer.tokenizeClauses(input, rel, pat, wit),
                          SyntaxError);
    }
}

TEST_CASE("QueryTokenizer: tokenizeClauses for with") {
    QueryTokenizer tokenizer;

    SECTION("PASS: synonym") {
        string input = "with name1 = name2";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<WithPair> expected =
            vector<WithPair>{make_pair("name1", "name2")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));

        wit.clear();
        input = "with name1=name2";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));
    }

    SECTION("PASS: integer") {
        string input = "with 123 = 456";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<WithPair> expected = vector<WithPair>{make_pair("123", "456")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));

        wit.clear();
        input = "with  123   =       456     ";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));
    }

    SECTION("PASS: quoted") {
        string input = "with \"something\" = \" a \"";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<WithPair> expected =
            vector<WithPair>{make_pair("\"something\"", "\"a\"")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));

        wit.clear();
        input = "with \" something3lse \" = \" a \"";
        expected = vector<WithPair>{make_pair("\"something3lse\"", "\"a\"")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));
    }

    SECTION("PASS: attribute") {
        string input = "with P.procName = v.varName";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<WithPair> expected =
            vector<WithPair>{make_pair("P.procName", "v.varName")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));

        wit.clear();
        input = "with P.procName = v . varName";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));

        wit.clear();
        input = "with P.procName = v .varName";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));

        wit.clear();
        input = "with P.procName = v. varName";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));

        wit.clear();
        input = "with P . procName = v . varName";
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));
    }

    SECTION("PASS: attribute and quoted") {
        string input = "with P . procName = \" a \"";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<WithPair> expected =
            vector<WithPair>{make_pair("P.procName", "\"a\"")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));

        wit.clear();
        input = "with \" a \"=P . procName";
        expected = vector<WithPair>{make_pair("\"a\"", "P.procName")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));
    }

    SECTION("PASS: multiple clauses") {
        string input = "with P . procName = \" a \" and a.stmt# = n";
        vector<ClsTuple> rel;
        vector<PatTuple> pat;
        vector<WithPair> wit;
        vector<WithPair> expected = vector<WithPair>{
            make_pair("P.procName", "\"a\""), make_pair("a.stmt#", "n")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));

        wit.clear();
        input = "with a.stmt# = n and P . procName = \" a \"";
        expected = vector<WithPair>{make_pair("a.stmt#", "n"),
                                    make_pair("P.procName", "\"a\"")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));

        wit.clear();
        input = "with a.stmt# = c.value with P . procName = \" a \"";
        expected = vector<WithPair>{make_pair("a.stmt#", "c.value"),
                                    make_pair("P.procName", "\"a\"")};
        tokenizer.tokenizeClauses(input, rel, pat, wit);
        REQUIRE(TestQueryTokenizer::compareVectorPair(wit, expected));
    }
}

TEST_CASE("QueryTokenizer: tokenizePattern") {
    QueryTokenizer tokenizer;

    SECTION("PASS: valid patterns tokenized") {
        vector<string> P_SIMPLE_1{"\"x + y\""};
        vector<string> P_SIMPLE_2{"\"x + 10 + y\""};
        vector<string> P_BRACKETS_1{"\"((y))\""};
        vector<string> P_BRACKETS_2{"\"x + ((y) * (x))\""};
        vector<string> P_COMPLEX_1{"\"x + y - 10 * (n4m3 % x) / y\""};
        vector<string> P_COMPLEX_2{
            "_\"(((x - y) * z) / (a - 20 % b) - 10) * x\"_"};

        vector<string> T_SIMPLE_1{"\"", "x", "+", "y", "\""};
        vector<string> T_SIMPLE_2{"\"", "x", "+", "10", "+", "y", "\""};
        vector<string> T_BRACKETS_1{"\"", "(", "(", "y", ")", ")", "\""};
        vector<string> T_BRACKETS_2{"\"", "x", "+", "(", "(", "y", ")",
                                    "*",  "(", "x", ")", ")", "\""};
        vector<string> T_COMPLEX_1{"\"",   "x", "+", "y", "-", "10", "*", "(",
                                   "n4m3", "%", "x", ")", "/", "y",  "\""};
        vector<string> T_COMPLEX_2{
            "_", "\"", "(", "(",  "(", "x", "-", "y",  ")",
            "*", "z",  ")", "/",  "(", "a", "-", "20", "%",
            "b", ")",  "-", "10", ")", "*", "x", "\"", "_",
        };

        vector<string> tokens = tokenizer.tokenizePattern(P_SIMPLE_1);
        REQUIRE(tokens == T_SIMPLE_1);

        tokens = tokenizer.tokenizePattern(P_SIMPLE_2);
        REQUIRE(tokens == T_SIMPLE_2);

        tokens = tokenizer.tokenizePattern(P_BRACKETS_1);
        REQUIRE(tokens == T_BRACKETS_1);

        tokens = tokenizer.tokenizePattern(P_BRACKETS_2);
        REQUIRE(tokens == T_BRACKETS_2);

        tokens = tokenizer.tokenizePattern(P_COMPLEX_1);
        REQUIRE(tokens == T_COMPLEX_1);

        tokens = tokenizer.tokenizePattern(P_COMPLEX_2);
        REQUIRE(tokens == T_COMPLEX_2);
    }

    SECTION("FAIL: mismatching brackets") {
        vector<string> P_BRACKETS_1{"(", "(", "y", ")"};
        vector<string> P_BRACKETS_2{"(", "y", ")", ")"};
        vector<string> P_BRACKETS_3{"(", "(", "y", ")", ")", "("};

        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_BRACKETS_1), SyntaxError);
        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_BRACKETS_2), SyntaxError);
        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_BRACKETS_3), SyntaxError);
    }

    SECTION("FAIL: invalid operator location") {
        vector<string> P_INVALID_1{"x", "+", "y", "-"};
        vector<string> P_INVALID_2{"x", "+", "-", "y"};
        vector<string> P_INVALID_3{"+", "x"};
        vector<string> P_INVALID_4{"+"};

        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_INVALID_1), SyntaxError);
        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_INVALID_2), SyntaxError);
        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_INVALID_3), SyntaxError);
        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_INVALID_4), SyntaxError);
    }

    SECTION("FAIL: invalid name/integer") {
        vector<string> P_INVALID_1{"x y"};
        vector<string> P_INVALID_2{"1234d"};

        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_INVALID_1), SyntaxError);
        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_INVALID_1), SyntaxError);
    }

    SECTION("FAIL: empty pattern string") {
        vector<string> P_EMPTY_1{"\"\""};
        vector<string> P_EMPTY_2{"_\"\"_"};

        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_EMPTY_1), SyntaxError);
        REQUIRE_THROWS_AS(tokenizer.tokenizePattern(P_EMPTY_2), SyntaxError);
    }
}

TEST_CASE("QueryTokenizer: syntax error checking") {
    QueryTokenizer tokenizer;
    string q;
    string remaining = "";
    vector<ClsTuple> rel;
    vector<PatTuple> pat;
    vector<WithPair> wit;

    SECTION("FAIL: empty tuple") {
        q = "Select <>";
        REQUIRE_THROWS_AS(tokenizer.tokenizeReturnSynonyms(q, remaining),
                          SyntaxError);
    }

    SECTION("FAIL: invalid clauses") {
        q = "and Parent(2, 3)";
        REQUIRE_THROWS_AS(tokenizer.tokenizeClauses(q, rel, pat, wit),
                          SyntaxError);
    }

    SECTION("FAIL: empty with arguments") {
        q = "with \" \" = n";
        REQUIRE_THROWS_AS(tokenizer.tokenizeClauses(q, rel, pat, wit),
                          SyntaxError);
        q = "with n = \" \"";
        REQUIRE_THROWS_AS(tokenizer.tokenizeClauses(q, rel, pat, wit),
                          SyntaxError);
        q = "with n = .";
        REQUIRE_THROWS_AS(tokenizer.tokenizeClauses(q, rel, pat, wit),
                          SyntaxError);
        q = "with . = n";
        REQUIRE_THROWS_AS(tokenizer.tokenizeClauses(q, rel, pat, wit),
                          SyntaxError);
    }

    SECTION("FAIL: empty quoted strings in such that clauses") {
        q = "such that Modifies(\"  \", \" y \")";
        REQUIRE_THROWS_AS(tokenizer.tokenizeClauses(q, rel, pat, wit),
                          SyntaxError);
        q = "such that Modifies(\" x \", \"  \")";
        REQUIRE_THROWS_AS(tokenizer.tokenizeClauses(q, rel, pat, wit),
                          SyntaxError);
    }

    SECTION("FAIL: empty quoted strings in pattern clauses") {
        q = "pattern a(\"  \", _)";
        REQUIRE_THROWS_AS(tokenizer.tokenizeClauses(q, rel, pat, wit),
                          SyntaxError);
    }
}
