#include "TestParserUtils.h"
#include "source_processor/parser/AssignStatementParser.h"

TEST_CASE("AssignStatementParser: parseAssignStatement") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = AssignStatementParser({"x", "=", "0", ";"}, INDEX).parseAssignStatement();
    auto testStmt = TestParserUtils::createAssignStmt(INDEX, "x", "0");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("AssignStatementParser: parseAssignStatement - throws invalid variable name") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = AssignStatementParser({" x", "=", "0", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parseAssignStatement());

    parser = AssignStatementParser({"x ", "=", "0", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parseAssignStatement());

    parser = AssignStatementParser({"1x", "=", "0", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parseAssignStatement());
}

TEST_CASE("AssignStatementParser: parseAssignStatement - throws invalid assign statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = AssignStatementParser({"x", "=", "0"}, INDEX);
    REQUIRE_THROWS(*parser.parseAssignStatement());

    parser = AssignStatementParser({"x", "=", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parseAssignStatement());

    parser = AssignStatementParser({"=", "0", ";"}, INDEX);
    REQUIRE_THROWS(*parser.parseAssignStatement());
}
