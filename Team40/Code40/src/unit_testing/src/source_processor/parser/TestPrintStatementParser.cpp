#include "TestParserUtils.h"
#include "source_processor/parser/PrintStatementParser.h"

TEST_CASE("PrintStatementParser: parsePrintStatement") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = PrintStatementParser({"print", "x", ";", }, INDEX).parsePrintStatement();
    auto testStmt = TestParserUtils::createPrintStmt(INDEX, "x");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("PrintStatementParser: parsePrintStatement - throws invalid variable name") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = PrintStatementParser({"print", " x", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parsePrintStatement());

    parser = PrintStatementParser({"print", "x ", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parsePrintStatement());

    parser = PrintStatementParser({"print", "1x", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parsePrintStatement());
}

TEST_CASE("PrintStatementParser: parsePrintStatement - throws invalid print statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = PrintStatementParser({"print", "x"}, INDEX);
    REQUIRE_THROWS(*parser.parsePrintStatement());

    parser = PrintStatementParser({"print", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parsePrintStatement());
}
