#include "catch.hpp"
#include "source_processor/SourceProcessor.h"
#include "pkb/PKB.h"
#include <string>

TEST_CASE("Parser") {
    PKB pkb;
    SourceProcessor sourceProcessor(&pkb);

    sourceProcessor.processSource("C:\\Users\\Admin\\source\\repos\\21s1-cp-spa-team-"
        "40\\Team40\\Tests40\\Source1.txt");
    int test = 0;
}