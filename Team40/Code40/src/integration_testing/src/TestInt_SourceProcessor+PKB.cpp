#include "catch.hpp"

#include "TestIntegrationUtils.h"
#include "pkb/PKB.h"
#include "source_processor/SourceProcessor.h"

using namespace std;

TEST_CASE("Integration: SP process Source1") {
    PKB pkb;
    SourceProcessor sourceProcessor(&pkb);
    sourceProcessor.processSource("../../../../../Tests40/basic1_source.txt");

    REQUIRE(pkb.getAllProcs().asVector().size() == 1); // Only one procedure

    string procName = "Examp1le";
    Procedure *actualProc = pkb.getProcByName(procName);
    Procedure testProc = TestIntegrationUtils::createExampleProcedure();

    auto testStmtList = testProc.getStmtLst();
    auto actualStmtList = actualProc->getStmtLst();

    REQUIRE(testStmtList.size() == actualStmtList.size());
    for (size_t i = 0; i < testStmtList.size(); i++) {
        // Statement contents are equal
        REQUIRE(*testStmtList[i] == *actualStmtList[i]);
    }
}
