#include "catch.hpp"

#include <string>

#include "pkb/PKB.h"

#include "source_processor/SourceProcessor.h"

TEST_CASE("Parser") {
    PKB pkb;
    SourceProcessor sourceProcessor(&pkb);

    sourceProcessor.processSource("C:\\Users\\Admin\\source\\repos\\21s1-cp-spa-team-"
        "40\\Team40\\Tests40\\Source1.txt");
    int test = 0;
}
