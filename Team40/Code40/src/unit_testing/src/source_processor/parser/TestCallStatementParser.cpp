#include "TestParserUtils.h"
#include "source_processor/parser/CallStatementParser.h"

TEST_CASE("CallStatementParser: parseCallStatement") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = CallStatementParser({"call", "x", ";", }, INDEX).parseCallStatement();
    auto testStmt = TestParserUtils::createCallStmt(INDEX, "x");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("CallStatementParser: parseCallStatement - throws invalid procedure name") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = CallStatementParser({"call", " x", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseCallStatement(), "invalid procedure name");

    parser = CallStatementParser({"call", "x ", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseCallStatement(),"invalid procedure name");

    parser = CallStatementParser({"call", "1x", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseCallStatement(),"invalid procedure name");

    parser = CallStatementParser({"call", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseCallStatement(), "invalid procedure name");
}

TEST_CASE("CallStatementParser: parseCallStatement - throws invalid call statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = CallStatementParser({"call", "x"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseCallStatement(), "invalid call statement");
}
