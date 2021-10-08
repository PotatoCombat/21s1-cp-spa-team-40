#include "catch.hpp"

#include <string>
#include <set>
#include <utility>

#include "query_processor/ResultTable.h"

TEST_CASE("ResultTable:") {
    SECTION("Add value:") {
        ResultTable table;
        table.clear();
        table.init(3);
        table.addValueWithLink(1, "x1", 2, {"x2", "x3"});
        VALUE_SET pointers = table.getPointersToIdx(1, "x1", 2);

        REQUIRE(pointers.size() == 2);
        REQUIRE(pointers.find("x2") != pointers.end());
        REQUIRE(pointers.find("x3") != pointers.end());
    }

    SECTION("Remove value:") {
        ResultTable table;
        table.clear();
        table.init(3);
        // line <=> related
        //   before 1    2   0        after  1    2   0
        //         a1 -  b1  c1                   b1
        //         a2 X  b2 /               a2 /
        table.addValueWithLink(1, "a1", 2, {"b1", "b2"});
        table.addValueWithLink(1, "a2", 2, {"b1"});
        table.addValueWithLink(2, "b1", 1, {"a1", "a2"});
        table.addValueWithLink(2, "b2", 1, {"a1"});
        table.addValueWithLink(2, "b2", 0, {"c1"});
        table.addValueWithLink(0, "c1", 2, {"b2"});

        table.removeValue(1, "a1");
        REQUIRE(table.getValues(1).size() == 1);
        REQUIRE(table.getValues(1).front() == "a2");
        REQUIRE(table.getValues(2).size() == 1);
        REQUIRE(table.getValues(2).front() == "b1");
        REQUIRE(table.getValues(0).size() == 0);
    }

    SECTION("Remove value 2:") {
        ResultTable table;
        table.clear();
        table.init(3);
        // line <=> related (a1 and c1 are connected)
        //   before 1    2   0        after  1    2   0
        //         a1 -  b1  c1                   b1
        //         a2 X  b2 /               a2 /
        table.addValueWithLink(1, "a1", 2, {"b1", "b2"});
        table.addValueWithLink(1, "a2", 2, {"b1"});
        table.addValueWithLink(2, "b1", 1, {"a1", "a2"});
        table.addValueWithLink(2, "b2", 1, {"a1"});
        table.addValueWithLink(2, "b2", 0, {"c1"});
        table.addValueWithLink(0, "c1", 2, {"b2"});
        table.addValueWithLink(1, "a1", 0, {"c1"});
        table.addValueWithLink(0, "c1", 1, {"a1"});

        table.removeValue(1, "a1");
        REQUIRE(table.getValues(1).size() == 1);
        REQUIRE(table.getValues(1).front() == "a2");
        REQUIRE(table.getValues(2).size() == 1);
        REQUIRE(table.getValues(2).front() == "b1");
        REQUIRE(table.empty(0));
    }
}
