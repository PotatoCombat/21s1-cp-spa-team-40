#include "TestParserUtils.h"
#include "source_processor/parser/CallStatementParser.h"

TEST_CASE("CallStatementParser: parseCallStatement") {
    int index = 1;
    auto *actualStmt = CallStatementParser({"call", "x", ";", }, index).parseCallStatement();
    auto testStmt = TestParserUtils::createCallStmt(index, "x");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("CallStatementParser: parseCallStatement - throws invalid variable name") {
    int index = 1;
    auto parser = CallStatementParser({"call", " x", ";"}, index);
    REQUIRE_THROWS(*parser.parseCallStatement());

    parser = CallStatementParser({"call", "x ", ";"}, index);
    REQUIRE_THROWS(*parser.parseCallStatement());

    parser = CallStatementParser({"call", "1x", ";"}, index);
    REQUIRE_THROWS(*parser.parseCallStatement());
}

TEST_CASE("CallStatementParser: parseCallStatement - throws invalid call statement") {
    int index = 1;
    auto parser = CallStatementParser({"call", "x"}, index);
    REQUIRE_THROWS(*parser.parseCallStatement());

    parser = CallStatementParser({"call", ";"}, index);
    REQUIRE_THROWS(*parser.parseCallStatement());
}
