#include "catch.hpp"
#include "source_processor/SourceProcessor.h"
#include "pkb/PKB.h"
#include <string>

TEST_CASE("Parser") {
    PKB pkb;
    SourceProcessor sourceProcessor(&pkb);

    sourceProcessor.processSource("C:\\Users\\Admin\\source\\repos\\21s1-cp-spa-team-"
        "40\\Team40\\Tests40\\TD_SIMPLE_2_source.txt");
    int test = 0;
}