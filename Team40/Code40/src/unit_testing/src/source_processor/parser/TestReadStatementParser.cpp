#include "TestParserUtils.h"
#include "source_processor/parser/ReadStatementParser.h"

TEST_CASE("ReadStatementParser: parseEntity") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = ReadStatementParser({"read", "x", ";", }, INDEX).parseEntity();
    auto testStmt = TestParserUtils::createReadStmt(INDEX, "x");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("ReadStatementParser: parseEntity - throws invalid variable name") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = ReadStatementParser({"read", " x", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");

    parser = ReadStatementParser({"read", "x ", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");

    parser = ReadStatementParser({"read", "1x", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");

    parser = ReadStatementParser({"read", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");
}

TEST_CASE("ReadStatementParser: parseEntity - throws invalid read statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = ReadStatementParser({"read", "x"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid read statement");
}
