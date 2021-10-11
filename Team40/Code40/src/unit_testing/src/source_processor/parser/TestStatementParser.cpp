#include "TestParserUtils.h"
#include "source_processor/parser/StatementParser.h"

vector<Line> programLines = {Line(0, {"procedure", "sumDigits", "{"}), 
                             Line(1, {"blah", ";"}),  
                             Line(1, {"}"})};
                             
TEST_CASE("StatementParser: isReadStmt") {
    int INDEX = TestParserUtils::INDEX;
    // True contents
    REQUIRE(StatementParser({"read", }, INDEX, programLines, INDEX).isReadStmt());
    REQUIRE(StatementParser({"read", "blah"}, INDEX, programLines, INDEX).isReadStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}, INDEX, programLines, INDEX).isReadStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }, INDEX, programLines, INDEX).isReadStmt());
    REQUIRE_FALSE(StatementParser({"blah", "read", }, INDEX, programLines, INDEX).isReadStmt());
    REQUIRE_FALSE(StatementParser({"reader", }, INDEX, programLines, INDEX).isReadStmt());
    REQUIRE_FALSE(StatementParser({" read", }, INDEX, programLines, INDEX).isReadStmt());
    REQUIRE_FALSE(StatementParser({"read ", }, INDEX, programLines, INDEX).isReadStmt());
    REQUIRE_FALSE(StatementParser({" read ", }, INDEX, programLines, INDEX).isReadStmt());
    REQUIRE_FALSE(StatementParser({"Read", }, INDEX, programLines, INDEX).isReadStmt());
    REQUIRE_FALSE(StatementParser({"READ", }, INDEX, programLines, INDEX).isReadStmt());
}

TEST_CASE("StatementParser: isPrintStmt") {
    int INDEX = TestParserUtils::INDEX;
    // True contents
    REQUIRE(StatementParser({"print", }, INDEX, programLines, INDEX).isPrintStmt());
    REQUIRE(StatementParser({"print", "blah"}, INDEX, programLines, INDEX).isPrintStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}, INDEX, programLines, INDEX).isPrintStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }, INDEX, programLines, INDEX).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"blah", "print", }, INDEX, programLines, INDEX).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"printr", }, INDEX, programLines, INDEX).isPrintStmt());
    REQUIRE_FALSE(StatementParser({" print", }, INDEX, programLines, INDEX).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"print ", }, INDEX, programLines, INDEX).isPrintStmt());
    REQUIRE_FALSE(StatementParser({" print ", }, INDEX, programLines, INDEX).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"Print", }, INDEX, programLines, INDEX).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"PRINT", }, INDEX, programLines, INDEX).isPrintStmt());
}

TEST_CASE("StatementParser: isCallStmt") {
    int INDEX = TestParserUtils::INDEX;
    // True contents
    REQUIRE(StatementParser({"call", }, INDEX, programLines, INDEX).isCallStmt());
    REQUIRE(StatementParser({"call", "blah"}, INDEX, programLines, INDEX).isCallStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}, INDEX, programLines, INDEX).isCallStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }, INDEX, programLines, INDEX).isCallStmt());
    REQUIRE_FALSE(StatementParser({"blah", "call", }, INDEX, programLines, INDEX).isCallStmt());
    REQUIRE_FALSE(StatementParser({"callr", }, INDEX, programLines, INDEX).isCallStmt());
    REQUIRE_FALSE(StatementParser({" call", }, INDEX, programLines, INDEX).isCallStmt());
    REQUIRE_FALSE(StatementParser({"call ", }, INDEX, programLines, INDEX).isCallStmt());
    REQUIRE_FALSE(StatementParser({" call ", }, INDEX, programLines, INDEX).isCallStmt());
    REQUIRE_FALSE(StatementParser({"Call", }, INDEX, programLines, INDEX).isCallStmt());
    REQUIRE_FALSE(StatementParser({"CALL", }, INDEX, programLines, INDEX).isCallStmt());
}

TEST_CASE("StatementParser: isWhileStmt") {
    int INDEX = TestParserUtils::INDEX;
    // True contents
    REQUIRE(StatementParser({"while", }, INDEX, programLines, INDEX).isWhileStmt());
    REQUIRE(StatementParser({"while", "blah"}, INDEX, programLines, INDEX).isWhileStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}, INDEX, programLines, INDEX).isWhileStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }, INDEX, programLines, INDEX).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"blah", "while", }, INDEX, programLines, INDEX).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"whiler", }, INDEX, programLines, INDEX).isWhileStmt());
    REQUIRE_FALSE(StatementParser({" while", }, INDEX, programLines, INDEX).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"while ", }, INDEX, programLines, INDEX).isWhileStmt());
    REQUIRE_FALSE(StatementParser({" while ", }, INDEX, programLines, INDEX).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"While", }, INDEX, programLines, INDEX).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"WHILE", }, INDEX, programLines, INDEX).isWhileStmt());
}

TEST_CASE("StatementParser: isIfStmt") {
    int INDEX = TestParserUtils::INDEX;
    // True contents
    REQUIRE(StatementParser({"if", }, INDEX, programLines, INDEX).isIfStmt());
    REQUIRE(StatementParser({"if", "blah"}, INDEX, programLines, INDEX).isIfStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}, INDEX, programLines, INDEX).isIfStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }, INDEX, programLines, INDEX).isIfStmt());
    REQUIRE_FALSE(StatementParser({"blah", "if", }, INDEX, programLines, INDEX).isIfStmt());
    REQUIRE_FALSE(StatementParser({"ifr", }, INDEX, programLines, INDEX).isIfStmt());
    REQUIRE_FALSE(StatementParser({" if", }, INDEX, programLines, INDEX).isIfStmt());
    REQUIRE_FALSE(StatementParser({"if ", }, INDEX, programLines, INDEX).isIfStmt());
    REQUIRE_FALSE(StatementParser({" if ", }, INDEX, programLines, INDEX).isIfStmt());
    REQUIRE_FALSE(StatementParser({"If", }, INDEX, programLines, INDEX).isIfStmt());
    REQUIRE_FALSE(StatementParser({"IF", }, INDEX, programLines, INDEX).isIfStmt());
}

TEST_CASE("StatementParser: isAssignStmt") {
    int INDEX = TestParserUtils::INDEX;
    // True contents
    REQUIRE(StatementParser({"x", "=", "5", }, INDEX, programLines, INDEX).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "+", "10", }, INDEX, programLines, INDEX).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "-", "10", }, INDEX, programLines, INDEX).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "*", "10", }, INDEX, programLines, INDEX).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "/", "10", }, INDEX, programLines, INDEX).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "%", "10", }, INDEX, programLines, INDEX).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "(", "5", "+", "10", ")", }, INDEX, programLines, INDEX).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "(", "5", "+", "10", ")", "*", "(", "5", "/", "10", ")"}, INDEX, programLines, INDEX).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "(", "(", "5", "+", "10", ")", "*", "(", "5", "/", "10", ")"}, INDEX, programLines, INDEX).isAssignStmt());

    // Empty Set
    REQUIRE_FALSE(StatementParser({}, INDEX, programLines, INDEX).isAssignStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"x", " =", "5", }, INDEX, programLines, INDEX).isAssignStmt());
    REQUIRE_FALSE(StatementParser({"x", "= ", "5", }, INDEX, programLines, INDEX).isAssignStmt());
    REQUIRE_FALSE(StatementParser({"x", " = ", "5", }, INDEX, programLines, INDEX).isAssignStmt());
}
