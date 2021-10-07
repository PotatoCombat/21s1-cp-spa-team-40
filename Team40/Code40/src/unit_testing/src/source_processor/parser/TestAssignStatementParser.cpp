#include "TestParserUtils.h"
#include "source_processor/parser/AssignStatementParser.h"

TEST_CASE("AssignStatementParser: parseAssignStatement") {
    int index = 1;
    auto *actualStmt = AssignStatementParser({"x", "=", "0", ";"}, index).parseAssignStatement();
    auto testStmt = TestParserUtils::createAssignStmt(index, "x", "0");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("AssignStatementParser: parseAssignStatement - throws invalid variable name") {
    int index = 1;
    auto parser = AssignStatementParser({" x", "=", "0", ";"}, index);
    REQUIRE_THROWS(*parser.parseAssignStatement());

    parser = AssignStatementParser({"x ", "=", "0", ";"}, index);
    REQUIRE_THROWS(*parser.parseAssignStatement());

    parser = AssignStatementParser({"1x", "=", "0", ";"}, index);
    REQUIRE_THROWS(*parser.parseAssignStatement());
}

TEST_CASE("AssignStatementParser: parseAssignStatement - throws invalid assign statement") {
    int index = 1;
    auto parser = AssignStatementParser({"x", "=", "0"}, index);
    REQUIRE_THROWS(*parser.parseAssignStatement());

    parser = AssignStatementParser({"x", "=", ";"}, index);
    REQUIRE_THROWS(*parser.parseAssignStatement());

    parser = AssignStatementParser({"=", "0", ";"}, index);
    REQUIRE_THROWS(*parser.parseAssignStatement());
}
