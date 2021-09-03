#include "catch.hpp"

#include "query_processor/query_preprocessor/QueryTokenizer.h"

string decl = "stmt s;";
string selectcl = "Select s";
string queryString("stmt s; Select s");
QueryTokenizer tokenizer;

string noWhitespaceString("stmt s;Select s");
string whitespaceString("  stmt s;     Select s     ");
string newlineString("  stmt s; \n\n    Select s     \n");

TEST_CASE("QP-QueryTokenizer: splitIntoParts") {
    pair<string, string> tokens = tokenizer.splitIntoParts(queryString);
    REQUIRE(tokens.first == decl);
    REQUIRE(tokens.second == selectcl);

    tokens = tokenizer.splitIntoParts(noWhitespaceString);
    REQUIRE(tokens.first == decl);
    REQUIRE(tokens.second == selectcl);

    tokens = tokenizer.splitIntoParts(whitespaceString);
    REQUIRE(tokens.first == decl);
    REQUIRE(tokens.second == selectcl);

    tokens = tokenizer.splitIntoParts(newlineString);
    REQUIRE(tokens.first == decl);
    REQUIRE(tokens.second == selectcl);

    REQUIRE_THROWS_WITH(tokenizer.splitIntoParts(decl), "Empty select clause");
    REQUIRE_THROWS_WITH(tokenizer.splitIntoParts(selectcl), "Empty declaration");
}

TEST_CASE("QP-QueryTokenizer: tokenizeDeclaration") {
    string expectedString = "stmt s";
    vector<string> declarations = tokenizer.tokenizeDeclaration(decl);
    REQUIRE(declarations[0] == expectedString);

    string decl1 = "stmt s; assign a; print p1;";
    vector<string> expected{ "stmt s", "assign a", "print p1" };
    vector<string> declarations1 = tokenizer.tokenizeDeclaration(decl1);
    REQUIRE(declarations1 == expected);
}
