#include "TestParserUtils.h"
#include "source_processor/parser/PrintStatementParser.h"

TEST_CASE("PrintStatementParser: parsePrintStatement") {
    int index = 1;
    auto *actualStmt = PrintStatementParser(
                           {
                               "print",
                               "x",
                               ";",
                           },
                           index)
                           .parsePrintStatement();
    auto testStmt = TestParserUtils::createPrintStmt(index, "x");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("PrintStatementParser: parsePrintStatement - throws invalid variable name") {
    int index = 1;
    auto parser = PrintStatementParser(
        {
            "print",
            " x",
        },
        index);
    REQUIRE_THROWS(*parser.parsePrintStatement());

    parser = PrintStatementParser(
        {
            "print",
            "x ",
        },
        index);
    REQUIRE_THROWS(*parser.parsePrintStatement());

    parser = PrintStatementParser(
        {
            "print",
            "1x",
        },
        index);
    REQUIRE_THROWS(*parser.parsePrintStatement());
}

TEST_CASE("PrintStatementParser: parsePrintStatement - throws invalid print statement") {
    int index = 1;
    auto parser = PrintStatementParser(
        {
            "print",
            "x",
        },
        index);
    REQUIRE_THROWS(*parser.parsePrintStatement());
}
