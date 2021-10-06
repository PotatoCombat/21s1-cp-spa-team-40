#include "TestParserUtils.h"
#include "source_processor/parser/StatementParser.h"

TEST_CASE("StatementParser: isReadStmt") {
    // True contents
    REQUIRE(StatementParser({"read", }).isReadStmt());
    REQUIRE(StatementParser({"read", "blah"}).isReadStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}).isReadStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }).isReadStmt());
    REQUIRE_FALSE(StatementParser({"blah", "read", }).isReadStmt());
    REQUIRE_FALSE(StatementParser({"reader", }).isReadStmt());
    REQUIRE_FALSE(StatementParser({" read", }).isReadStmt());
    REQUIRE_FALSE(StatementParser({"read ", }).isReadStmt());
    REQUIRE_FALSE(StatementParser({" read ", }).isReadStmt());
    REQUIRE_FALSE(StatementParser({"Read", }).isReadStmt());
    REQUIRE_FALSE(StatementParser({"READ", }).isReadStmt());
}

TEST_CASE("StatementParser: isPrintStmt") {
    // True contents
    REQUIRE(StatementParser({"print", }).isPrintStmt());
    REQUIRE(StatementParser({"print", "blah"}).isPrintStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}).isPrintStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"blah", "print", }).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"printr", }).isPrintStmt());
    REQUIRE_FALSE(StatementParser({" print", }).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"print ", }).isPrintStmt());
    REQUIRE_FALSE(StatementParser({" print ", }).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"Print", }).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"PRINT", }).isPrintStmt());
}

TEST_CASE("StatementParser: isCallStmt") {
    // True contents
    REQUIRE(StatementParser({"call", }).isCallStmt());
    REQUIRE(StatementParser({"call", "blah"}).isCallStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}).isCallStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }).isCallStmt());
    REQUIRE_FALSE(StatementParser({"blah", "call", }).isCallStmt());
    REQUIRE_FALSE(StatementParser({"callr", }).isCallStmt());
    REQUIRE_FALSE(StatementParser({" call", }).isCallStmt());
    REQUIRE_FALSE(StatementParser({"call ", }).isCallStmt());
    REQUIRE_FALSE(StatementParser({" call ", }).isCallStmt());
    REQUIRE_FALSE(StatementParser({"Call", }).isCallStmt());
    REQUIRE_FALSE(StatementParser({"CALL", }).isCallStmt());
}

TEST_CASE("StatementParser: isWhileStmt") {
    // True contents
    REQUIRE(StatementParser({"while", }).isWhileStmt());
    REQUIRE(StatementParser({"while", "blah"}).isWhileStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}).isWhileStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"blah", "while", }).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"whiler", }).isWhileStmt());
    REQUIRE_FALSE(StatementParser({" while", }).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"while ", }).isWhileStmt());
    REQUIRE_FALSE(StatementParser({" while ", }).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"While", }).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"WHILE", }).isWhileStmt());
}

TEST_CASE("StatementParser: isIfStmt") {
    // True contents
    REQUIRE(StatementParser({"if", }).isIfStmt());
    REQUIRE(StatementParser({"if", "blah"}).isIfStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}).isIfStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }).isIfStmt());
    REQUIRE_FALSE(StatementParser({"blah", "if", }).isIfStmt());
    REQUIRE_FALSE(StatementParser({"ifr", }).isIfStmt());
    REQUIRE_FALSE(StatementParser({" if", }).isIfStmt());
    REQUIRE_FALSE(StatementParser({"if ", }).isIfStmt());
    REQUIRE_FALSE(StatementParser({" if ", }).isIfStmt());
    REQUIRE_FALSE(StatementParser({"If", }).isIfStmt());
    REQUIRE_FALSE(StatementParser({"IF", }).isIfStmt());
}

TEST_CASE("StatementParser: isAssignStmt") {
    // True contents
    REQUIRE(StatementParser({"x", "=", "5", }).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "+", "10", }).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "-", "10", }).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "*", "10", }).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "/", "10", }).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "%", "10", }).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "(", "5", "+", "10", ")", }).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "(", "5", "+", "10", ")", "*", "(", "5", "/", "10", ")"}).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "(", "(", "5", "+", "10", ")", "*", "(", "5", "/", "10", ")"}).isAssignStmt());

    // Empty Set
    REQUIRE_FALSE(StatementParser({}).isAssignStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"x", " =", "5", }).isAssignStmt());
    REQUIRE_FALSE(StatementParser({"x", "= ", "5", }).isAssignStmt());
    REQUIRE_FALSE(StatementParser({"x", " = ", "5", }).isAssignStmt());
}
