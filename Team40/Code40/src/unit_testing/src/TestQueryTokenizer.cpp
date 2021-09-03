#include "catch.hpp"

#include "query_processor/query_preprocessor/QueryTokenizer.h"

QueryTokenizer tokenizer;

string decl = "stmt s;";
string selectcl = "Select s";
string queryString("stmt s; Select s");

string decl1 = "stmt s; assign a; print p1;";
string selectcl1 = "Select s such that Follows(s, p1)";
string queryString1("stmt s; assign a; print p1;\nSelect s such that Follows(s, p1)");

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

    tokens = tokenizer.splitIntoParts(queryString1);
    REQUIRE(tokens.first == decl1);
    REQUIRE(tokens.second == selectcl1);

    REQUIRE_THROWS_WITH(tokenizer.splitIntoParts(decl), "Empty select clause");
    REQUIRE_THROWS_WITH(tokenizer.splitIntoParts(selectcl), "Empty declaration");
}

TEST_CASE("QP-QueryTokenizer: tokenizeDeclaration") {
    string expectedString = "stmt s";
    vector<string> declarations = tokenizer.tokenizeDeclaration(decl);
    REQUIRE(declarations[0] == expectedString);

    vector<string> expected{ "stmt s", "assign a", "print p1" };
    vector<string> declarations1 = tokenizer.tokenizeDeclaration(decl1);
    REQUIRE(declarations1 == expected);
}

TEST_CASE("QP-QueryTokenizer: tokenizeSelectClause") {
    string expectedReturn;
    pair<string, vector<string>> selectClause;
    string expectedClause;

    expectedReturn = "s";
    selectClause = tokenizer.tokenizeSelectClause(selectcl);
    REQUIRE(selectClause.first == expectedReturn);
    REQUIRE(selectClause.second == vector<string>());
    
    expectedClause = "Follows(s, p1)";
    selectClause = tokenizer.tokenizeSelectClause(selectcl1);
    REQUIRE(selectClause.first == expectedReturn);
    REQUIRE(selectClause.second[0] == expectedClause);
}
