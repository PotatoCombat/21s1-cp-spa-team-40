#include "TestParserUtils.h"
#include "source_processor/parser/PrintStatementParser.h"

TEST_CASE("PrintStatementParser: parseEntity") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = PrintStatementParser({"print", "x", ";", }, INDEX).parseEntity();
    auto testStmt = TestParserUtils::createPrintStmt(INDEX, "x");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("PrintStatementParser: parseEntity - throws invalid variable name") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = PrintStatementParser({"print", " x", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");

    parser = PrintStatementParser({"print", "x ", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");

    parser = PrintStatementParser({"print", "1x", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");

    parser = PrintStatementParser({"print", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");
}

TEST_CASE("PrintStatementParser: parseEntity - throws invalid print statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = PrintStatementParser({"print", "x"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid print statement");
}
