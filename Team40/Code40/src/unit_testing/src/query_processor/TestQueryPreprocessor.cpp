#include "catch.hpp"

#include <string>
#include <iostream>

#include "Query.h"
#include "QueryPreprocessor.h"

struct TestQProcessor {
    static const string INPUT_1;
    //static const string INPUT_2;
    //static const string INPUT_3;
};

const string TestQProcessor::INPUT_1 =
    "stmt s;\nSelect s such that Follows(s, 4)";
//const string TestQProcessor::INPUT_2 =
//    "stmt s;\nSelect s";
//const string TestQProcessor::INPUT_3 = 
//    "stmt s;\nSelect p";

TEST_CASE("QueryPreprocessor") { 
    Query expected;
    Reference ret(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
    Reference ref(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
    Clause cls(ClauseType::FOLLOWS, ret, ref);
    expected.setReturnReference(&ret);
    expected.addClause(&cls);

    QueryPreprocessor qp;
    Query actual;
    
    SECTION("test query created correctly") {
        qp.preprocessQuery(TestQProcessor::INPUT_1, actual);
        /*cout << (*actual.getClauses()[0]).getFirstReference()->getValue() << endl;
        cout << (cls.getFirstReference()->getValue()) << endl;
        cout << (*actual.getClauses()[0]).getSecondReference()->getValue() << endl;
        cout << (cls.getSecondReference()->getValue()) << endl;*/

        //REQUIRE((*actual.getClauses()[0]).equals(*cls));
        REQUIRE((actual.getReturnReference()->equals(ret)));
    }
}
