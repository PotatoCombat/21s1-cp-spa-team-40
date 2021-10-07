#include "TestParserUtils.h"
#include "source_processor/parser/StatementParser.h"

int index = 1;
vector<Line> programLines = {Line(0, {"procedure", "sumDigits", "{"}), 
                             Line(1, {"blah", ";"}),  
                             Line(1, {"}"})};
                             
TEST_CASE("StatementParser: isReadStmt") {
    // True contents
    REQUIRE(StatementParser({"read", }, index, programLines, index).isReadStmt());
    REQUIRE(StatementParser({"read", "blah"}, index, programLines, index).isReadStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}, index, programLines, index).isReadStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }, index, programLines, index).isReadStmt());
    REQUIRE_FALSE(StatementParser({"blah", "read", }, index, programLines, index).isReadStmt());
    REQUIRE_FALSE(StatementParser({"reader", }, index, programLines, index).isReadStmt());
    REQUIRE_FALSE(StatementParser({" read", }, index, programLines, index).isReadStmt());
    REQUIRE_FALSE(StatementParser({"read ", }, index, programLines, index).isReadStmt());
    REQUIRE_FALSE(StatementParser({" read ", }, index, programLines, index).isReadStmt());
    REQUIRE_FALSE(StatementParser({"Read", }, index, programLines, index).isReadStmt());
    REQUIRE_FALSE(StatementParser({"READ", }, index, programLines, index).isReadStmt());
}

TEST_CASE("StatementParser: isPrintStmt") {
    // True contents
    REQUIRE(StatementParser({"print", }, index, programLines, index).isPrintStmt());
    REQUIRE(StatementParser({"print", "blah"}, index, programLines, index).isPrintStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}, index, programLines, index).isPrintStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }, index, programLines, index).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"blah", "print", }, index, programLines, index).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"printr", }, index, programLines, index).isPrintStmt());
    REQUIRE_FALSE(StatementParser({" print", }, index, programLines, index).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"print ", }, index, programLines, index).isPrintStmt());
    REQUIRE_FALSE(StatementParser({" print ", }, index, programLines, index).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"Print", }, index, programLines, index).isPrintStmt());
    REQUIRE_FALSE(StatementParser({"PRINT", }, index, programLines, index).isPrintStmt());
}

TEST_CASE("StatementParser: isCallStmt") {
    // True contents
    REQUIRE(StatementParser({"call", }, index, programLines, index).isCallStmt());
    REQUIRE(StatementParser({"call", "blah"}, index, programLines, index).isCallStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}, index, programLines, index).isCallStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }, index, programLines, index).isCallStmt());
    REQUIRE_FALSE(StatementParser({"blah", "call", }, index, programLines, index).isCallStmt());
    REQUIRE_FALSE(StatementParser({"callr", }, index, programLines, index).isCallStmt());
    REQUIRE_FALSE(StatementParser({" call", }, index, programLines, index).isCallStmt());
    REQUIRE_FALSE(StatementParser({"call ", }, index, programLines, index).isCallStmt());
    REQUIRE_FALSE(StatementParser({" call ", }, index, programLines, index).isCallStmt());
    REQUIRE_FALSE(StatementParser({"Call", }, index, programLines, index).isCallStmt());
    REQUIRE_FALSE(StatementParser({"CALL", }, index, programLines, index).isCallStmt());
}

TEST_CASE("StatementParser: isWhileStmt") {
    // True contents
    REQUIRE(StatementParser({"while", }, index, programLines, index).isWhileStmt());
    REQUIRE(StatementParser({"while", "blah"}, index, programLines, index).isWhileStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}, index, programLines, index).isWhileStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }, index, programLines, index).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"blah", "while", }, index, programLines, index).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"whiler", }, index, programLines, index).isWhileStmt());
    REQUIRE_FALSE(StatementParser({" while", }, index, programLines, index).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"while ", }, index, programLines, index).isWhileStmt());
    REQUIRE_FALSE(StatementParser({" while ", }, index, programLines, index).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"While", }, index, programLines, index).isWhileStmt());
    REQUIRE_FALSE(StatementParser({"WHILE", }, index, programLines, index).isWhileStmt());
}

TEST_CASE("StatementParser: isIfStmt") {
    // True contents
    REQUIRE(StatementParser({"if", }, index, programLines, index).isIfStmt());
    REQUIRE(StatementParser({"if", "blah"}, index, programLines, index).isIfStmt());

    // Empty Set
    REQUIRE_THROWS(StatementParser({}, index, programLines, index).isIfStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"blah", }, index, programLines, index).isIfStmt());
    REQUIRE_FALSE(StatementParser({"blah", "if", }, index, programLines, index).isIfStmt());
    REQUIRE_FALSE(StatementParser({"ifr", }, index, programLines, index).isIfStmt());
    REQUIRE_FALSE(StatementParser({" if", }, index, programLines, index).isIfStmt());
    REQUIRE_FALSE(StatementParser({"if ", }, index, programLines, index).isIfStmt());
    REQUIRE_FALSE(StatementParser({" if ", }, index, programLines, index).isIfStmt());
    REQUIRE_FALSE(StatementParser({"If", }, index, programLines, index).isIfStmt());
    REQUIRE_FALSE(StatementParser({"IF", }, index, programLines, index).isIfStmt());
}

TEST_CASE("StatementParser: isAssignStmt") {
    // True contents
    REQUIRE(StatementParser({"x", "=", "5", }, index, programLines, index).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "+", "10", }, index, programLines, index).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "-", "10", }, index, programLines, index).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "*", "10", }, index, programLines, index).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "/", "10", }, index, programLines, index).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "5", "%", "10", }, index, programLines, index).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "(", "5", "+", "10", ")", }, index, programLines, index).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "(", "5", "+", "10", ")", "*", "(", "5", "/", "10", ")"}, index, programLines, index).isAssignStmt());
    REQUIRE(StatementParser({"x", "=", "(", "(", "5", "+", "10", ")", "*", "(", "5", "/", "10", ")"}, index, programLines, index).isAssignStmt());

    // Empty Set
    REQUIRE_FALSE(StatementParser({}, index, programLines, index).isAssignStmt());

    // False contents
    REQUIRE_FALSE(StatementParser({"x", " =", "5", }, index, programLines, index).isAssignStmt());
    REQUIRE_FALSE(StatementParser({"x", "= ", "5", }, index, programLines, index).isAssignStmt());
    REQUIRE_FALSE(StatementParser({"x", " = ", "5", }, index, programLines, index).isAssignStmt());
}
