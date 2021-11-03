#include "TestParserUtils.h"
#include "source_processor/parser/AssignStatementParser.h"

TEST_CASE("AssignStatementParser: parseEntity") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = AssignStatementParser({"x", "=", "0", ";"}, INDEX).parseEntity();
    auto testStmt = TestParserUtils::createAssignStmt(INDEX, "x", "0");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("AssignStatementParser: parseEntity - throws invalid variable name") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = AssignStatementParser({" x", "=", "0", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");

    parser = AssignStatementParser({"x ", "=", "0", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");

    parser = AssignStatementParser({"1x", "=", "0", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid variable name");
}

TEST_CASE("AssignStatementParser: parseEntity - throws invalid assign statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = AssignStatementParser({"x", "=", "0"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid assign statement");

    parser = AssignStatementParser({"x", "=", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(),"invalid expression: no arguments");

    parser = AssignStatementParser({"=", "0", ";"}, INDEX);
    REQUIRE_THROWS_WITH(*parser.parseEntity(),"invalid assign statement");
}
