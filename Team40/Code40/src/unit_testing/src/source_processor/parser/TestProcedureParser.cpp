#include "TestParserUtils.h"
#include "source_processor/parser/ProcedureParser.h"

TEST_CASE("ProcedureParser: parseProcedure") {
    int index = 1;
    auto *actualStmt = ProcedureParser({"procedure", "computeAverage", "{"}).parseProcedure();
    auto testStmt = TestParserUtils::createProcedure("computeAverage");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("ProcedureParser: parseProcedure - throws invalid procedure name") {
    int index = 1;
    auto parser = ProcedureParser({"procedure", " computeAverage", "{"});
    REQUIRE_THROWS(*parser.parseProcedure());

    parser = ProcedureParser({"procedure", "computeAverage ", "{"});
    REQUIRE_THROWS(*parser.parseProcedure());

    parser = ProcedureParser({"procedure", "1computeAverage", "{"});
    REQUIRE_THROWS(*parser.parseProcedure());
}

TEST_CASE("ProcedureParser: parseProcedure - throws invalid print statement") {
    int index = 1;
    auto parser = ProcedureParser({"procedure", "{"});
    REQUIRE_THROWS(*parser.parseProcedure());

    parser = ProcedureParser({"procedure", "computeAverage"});
    REQUIRE_THROWS(*parser.parseProcedure());
}
