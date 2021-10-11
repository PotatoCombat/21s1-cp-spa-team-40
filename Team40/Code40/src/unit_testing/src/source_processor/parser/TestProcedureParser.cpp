#include "TestParserUtils.h"
#include "source_processor/parser/ProcedureParser.h"

TEST_CASE("ProcedureParser: parseProcedure") {
    auto *actualStmt = ProcedureParser({"procedure", "computeAverage", "{"}).parseProcedure();
    auto testStmt = TestParserUtils::createProcedure("computeAverage");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("ProcedureParser: parseProcedure - throws invalid procedure name") {
    auto parser = ProcedureParser({"procedure", " computeAverage", "{"});
    REQUIRE_THROWS_WITH(*parser.parseProcedure(), "invalid procedure name");

    parser = ProcedureParser({"procedure", "computeAverage ", "{"});
    REQUIRE_THROWS_WITH(*parser.parseProcedure(), "invalid procedure name");

    parser = ProcedureParser({"procedure", "1computeAverage", "{"});
    REQUIRE_THROWS_WITH(*parser.parseProcedure(), "invalid procedure name");

    parser = ProcedureParser({"procedure", "{"});
    REQUIRE_THROWS_WITH(*parser.parseProcedure(), "invalid procedure name");
}

TEST_CASE("ProcedureParser: parseProcedure - throws invalid procedure") {
    auto parser = ProcedureParser({"procedure", "computeAverage"});
    REQUIRE_THROWS_WITH(*parser.parseProcedure(), "invalid procedure");
}
