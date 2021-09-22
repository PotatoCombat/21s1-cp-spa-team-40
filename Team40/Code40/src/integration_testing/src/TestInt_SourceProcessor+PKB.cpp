#include "catch.hpp"

#include <string>
#include <vector>

#include "pkb/PKB.h"
#include "source_processor/SourceProcessor.h"

using namespace std;

TEST_CASE("Integration: SP read statements") {
    PKB pkb;
    SourceProcessor sourceProcessor(&pkb);

    sourceProcessor.processSource("../../../../Tests40/TD_SIMPLE_2_source.txt");

    vector<ProcName> procedures{"computeAverage"};
    vector<ProcName> results = pkb.getAllProcs().asVector();

    for (int i = 0; i < results.size(); i++) {
        REQUIRE(results.at(i) == procedures.at(i));
    }
}
