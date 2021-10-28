#include "TestParserUtils.h"
#include "source_processor/parser/ProcedureParser.h"

TEST_CASE("ProcedureParser: parseEntity") {
    auto *actualProc = ProcedureParser({"procedure", "computeAverage", "{"}).parseEntity();
    auto testProc = TestParserUtils::createProcedure("computeAverage");
    REQUIRE(*actualProc == testProc);
    delete actualProc;
}

TEST_CASE("ProcedureParser: parseEntity - throws invalid procedure name") {
    auto parser = ProcedureParser({"procedure", " computeAverage", "{"});
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid procedure name");

    parser = ProcedureParser({"procedure", "computeAverage ", "{"});
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid procedure name");

    parser = ProcedureParser({"procedure", "1computeAverage", "{"});
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid procedure name");

    parser = ProcedureParser({"procedure", "{"});
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid procedure name");
}

TEST_CASE("ProcedureParser: parseEntity - throws invalid procedure") {
    auto parser = ProcedureParser({"procedure", "computeAverage"});
    REQUIRE_THROWS_WITH(*parser.parseEntity(), "invalid procedure");
}
