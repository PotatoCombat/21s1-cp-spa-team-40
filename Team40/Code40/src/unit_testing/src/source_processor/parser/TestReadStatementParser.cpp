#include "TestParserUtils.h"
#include "source_processor/parser/ReadStatementParser.h"

TEST_CASE("ReadStatementParser: parseReadStatement") {
    int index = 1;
    auto *actualStmt = ReadStatementParser(
                           {
                               "read",
                               "x",
                               ";",
                           },
                           index)
                           .parseReadStatement();
    auto testStmt = TestParserUtils::createReadStmt(index, "x");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("ReadStatementParser: parseReadStatement - throws invalid variable name") {
    int index = 1;
    auto parser = ReadStatementParser(
        {
            "read",
            " x",
        },
        index);
    REQUIRE_THROWS(*parser.parseReadStatement());

    parser = ReadStatementParser(
        {
            "read",
            "x ",
        },
        index);
    REQUIRE_THROWS(*parser.parseReadStatement());

    parser = ReadStatementParser(
        {
            "read",
            "1x",
        },
        index);
    REQUIRE_THROWS(*parser.parseReadStatement());
}

TEST_CASE("ReadStatementParser: parseReadStatement - throws invalid print statement") {
    int index = 1;
    auto parser = ReadStatementParser(
        {
            "read",
            "x",
        },
        index);
    REQUIRE_THROWS(*parser.parseReadStatement());
}
