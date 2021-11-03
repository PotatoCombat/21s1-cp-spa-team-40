#include "TestParserUtils.h"
#include "source_processor/parser/CallStatementParser.h"

TEST_CASE("CallStatementParser: parseEntity") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = CallStatementParser({"call", "x", ";", }, INDEX).parseEntity();
    auto testStmt = TestParserUtils::createCallStmt(INDEX, "x");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("CallStatementParser: parseEntity - throws invalid procedure name") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = CallStatementParser({"call", " x", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid procedure name");

    parser = CallStatementParser({"call", "x ", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(),"invalid procedure name");

    parser = CallStatementParser({"call", "1x", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(),"invalid procedure name");

    parser = CallStatementParser({"call", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid procedure name");
}

TEST_CASE("CallStatementParser: parseEntity - throws invalid call statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = CallStatementParser({"call", "x"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid call statement");
}
