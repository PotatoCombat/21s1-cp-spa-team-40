#include "TestParserUtils.h"
#include "source_processor/parser/IfStatementParser.h"

// TEST_CASE("IfStatementParser: parseIfStatement") {
//     int index = 1;
//     auto *actualStmt = IfStatementParser({"x", "=", "0", }, index).parseIfStatement();
//     auto testStmt = TestParserUtils::createIfStmt(index, "x", "0");
//     REQUIRE(*actualStmt == testStmt);
//     delete actualStmt;
// }

// TEST_CASE("IfStatementParser: parseIfStatement - throws invalid variable name") {
//     int index = 1;
//     auto parser = IfStatementParser({" x", "=", "0", ";"}, index);
//     REQUIRE_THROWS(*parser.parseIfStatement());

//     parser = IfStatementParser({"x ", "=", "0", ";"}, index);
//     REQUIRE_THROWS(*parser.parseIfStatement());

//     parser = IfStatementParser({"1x", "=", "0", ";"}, index);
//     REQUIRE_THROWS(*parser.parseIfStatement());
// }

// TEST_CASE("IfStatementParser: parseIfStatement - throws invalid if statement") {
//     int index = 1;
//     auto parser = IfStatementParser({"x", "=", "0"}, index);
//     REQUIRE_THROWS(*parser.parseIfStatement());

//     auto parser = IfStatementParser({"x", "=", ";"}, index);
//     REQUIRE_THROWS(*parser.parseIfStatement());

//     auto parser = IfStatementParser({"=", "0", ";"}, index);
//     REQUIRE_THROWS(*parser.parseIfStatement());
// }
