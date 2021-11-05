#include "TestPKBUtils.h"

#include "pkb/PostfixAdapter.h"

#include "catch.hpp"

using namespace std;

TEST_CASE("PostfixAdapter: postfix partial patterns algorithm") {
    auto postfix = PostfixAdapter(TestPKBUtils::INFIX_1);
    REQUIRE(TestUtils::equalSets(
            postfix.createPartialPatterns(), TestPKBUtils::PARTIAL_1));

    postfix = PostfixAdapter(TestPKBUtils::INFIX_2);
    REQUIRE(TestUtils::equalSets(
            postfix.createPartialPatterns(), TestPKBUtils::PARTIAL_2));

    postfix = PostfixAdapter(TestPKBUtils::INFIX_3);
    REQUIRE(TestUtils::equalSets(
            postfix.createPartialPatterns(), TestPKBUtils::PARTIAL_3));

    postfix = PostfixAdapter(TestPKBUtils::INFIX_4);
    REQUIRE(TestUtils::equalSets(
            postfix.createPartialPatterns(), TestPKBUtils::PARTIAL_4));
}

TEST_CASE("PatternTable: postfix exact pattern algorithm") {
    auto postfix = PostfixAdapter(TestPKBUtils::INFIX_1);
    REQUIRE(postfix.createExactPattern() == TestPKBUtils::EXACT_1);

    postfix = PostfixAdapter(TestPKBUtils::INFIX_2);
    REQUIRE(postfix.createExactPattern() == TestPKBUtils::EXACT_2);

    postfix = PostfixAdapter(TestPKBUtils::INFIX_3);
    REQUIRE(postfix.createExactPattern() == TestPKBUtils::EXACT_3);

    postfix = PostfixAdapter(TestPKBUtils::INFIX_4);
    REQUIRE(postfix.createExactPattern() == TestPKBUtils::EXACT_4);
}
