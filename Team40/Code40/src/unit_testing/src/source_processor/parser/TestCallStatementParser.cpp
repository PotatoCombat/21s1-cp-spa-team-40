#include "TestParserUtils.h"
#include "source_processor/parser/CallStatementParser.h"

TEST_CASE("CallStatementParser: parseCallStatement") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = CallStatementParser({"call", "x", ";", }, INDEX).parseCallStatement();
    auto testStmt = TestParserUtils::createCallStmt(INDEX, "x");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("CallStatementParser: parseCallStatement - throws invalid variable name") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = CallStatementParser({"call", " x", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parseCallStatement());

    parser = CallStatementParser({"call", "x ", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parseCallStatement());

    parser = CallStatementParser({"call", "1x", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parseCallStatement());
}

TEST_CASE("CallStatementParser: parseCallStatement - throws invalid call statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = CallStatementParser({"call", "x"}, INDEX);
    REQUIRE_THROWS(*parser.parseCallStatement());

    parser = CallStatementParser({"call", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parseCallStatement());
}
