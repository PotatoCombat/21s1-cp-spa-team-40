#include "TestParserUtils.h"
#include "source_processor/parser/ReadStatementParser.h"

TEST_CASE("ReadStatementParser: parseReadStatement") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = ReadStatementParser({"read", "x", ";", }, INDEX).parseReadStatement();
    auto testStmt = TestParserUtils::createReadStmt(INDEX, "x");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("ReadStatementParser: parseReadStatement - throws invalid variable name") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = ReadStatementParser({"read", " x", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseReadStatement(), "invalid variable name");

    parser = ReadStatementParser({"read", "x ", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseReadStatement(), "invalid variable name");

    parser = ReadStatementParser({"read", "1x", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseReadStatement(), "invalid variable name");

    parser = ReadStatementParser({"read", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseReadStatement(), "invalid variable name");
}

TEST_CASE("ReadStatementParser: parseReadStatement - throws invalid read statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = ReadStatementParser({"read", "x"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseReadStatement(), "invalid read statement");
}
