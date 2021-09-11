#include "catch.hpp"

#include <vector>
#include <string>

#include "source_processor/SourceProcessor.h"
#include "pkb/PKB.h"

using namespace std;

TEST_CASE("Integration: SP read statements") {
    PKB pkb;
    SourceProcessor sourceProcessor(&pkb);

    sourceProcessor.processSource("../../../../Tests40/source_read1.txt");

    vector<ProcName> procedures{"computeAverage"};
    vector<ProcName> results = pkb.getAllProcs().asVector();

    for (int i = 0; i < results.size(); i++) {
        REQUIRE(results.at(i) == procedures.at(i));
    }
}
