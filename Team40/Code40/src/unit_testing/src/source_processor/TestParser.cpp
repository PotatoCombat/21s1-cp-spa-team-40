#include "catch.hpp"
#include "spa/SPA.h"
#include <string>

TEST_CASE("Parser") {
    SPA spa;
    spa.processSource("C:\\Users\\Admin\\source\\repos\\21s1-cp-spa-team-"
                      "40\\Team40\\Tests40\\Code4.txt");
}
