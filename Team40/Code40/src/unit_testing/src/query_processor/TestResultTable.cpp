#include "catch.hpp"

#include <string>
#include <set>
#include <utility>

#include "query_processor/ResultTable.h"
#include "query_processor/ValueToPointersMap.h"

TEST_CASE("ResultTable:") {
    SECTION("Add value:") {
        ResultTable table;
        table.clear();
        table.init(3);
        table.addValue(1, "x1", set<pair<int, string>>{make_pair(2, "x2")});
        table.addValue(1, "x1", set<pair<int, string>>{make_pair(2, "x3")});
        set<pair<int, string>> pointers = table.getPointers(1, "x1");
        
        REQUIRE(pointers.size() == 2);
        REQUIRE(pointers.find(make_pair(2, "x2")) != pointers.end());
        REQUIRE(pointers.find(make_pair(2, "x3")) != pointers.end());
    }

    SECTION("Remove value:") {
        ResultTable table;
        table.clear();
        table.init(3);
        // line <=> related
        //   before 1    2   0        after  1    2   0 
        //         a1 -  b1  c1                   b1 
        //         a2 X  b2 /               a2 /  
        table.addValue(1, "a1", set<pair<int, string>>{make_pair(2, "b1")});
        table.addValue(2, "b1", set<pair<int, string>>{make_pair(1, "a1")});
        table.addValue(1, "a1", set<pair<int, string>>{make_pair(2, "b2")});
        table.addValue(2, "b2", set<pair<int, string>>{make_pair(1, "a1")});
        table.addValue(1, "a2", set<pair<int, string>>{make_pair(2, "b1")});
        table.addValue(2, "b1", set<pair<int, string>>{make_pair(1, "a2")});
        table.addValue(0, "c1", set<pair<int, string>>{make_pair(2, "b2")});
        table.addValue(2, "b2", set<pair<int, string>>{make_pair(0, "c1")});
        
        table.removeMap(1, "a1");
        REQUIRE(table.getValues(1).size() == 1);
        REQUIRE(table.getValues(1).front() == "a2");
        REQUIRE(table.getValues(2).size() == 1);
        REQUIRE(table.getValues(2).front() == "b1");
        REQUIRE(table.getValues(0).size() == 0);
    }
}