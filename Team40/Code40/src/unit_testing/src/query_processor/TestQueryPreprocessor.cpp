#include "catch.hpp"

#include <string>
#include <iostream>

#include "Query.h"
#include "query_processor/QueryPreprocessor.h"

struct TestQPreprocessor {
    static const string INPUT_1;
    static const string INPUT_2;
    static const string INPUT_3;
};

const string TestQPreprocessor::INPUT_1 =
    "stmt s;\nSelect s such that Follows(s, 4)";
const string TestQPreprocessor::INPUT_2 =
    "stmt s;\nSelect s";
const string TestQPreprocessor::INPUT_3 = 
    "stmt s;\nSelect p";

TEST_CASE("QueryPreprocessor") { 
    QueryPreprocessor qp;
    
    SECTION("test query created correctly") {
        Query expected;
        Query actual;
        Reference ret(DesignEntityType::STMT, ReferenceType::SYNONYM, "s");
        Reference ref(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
        
        SECTION("test 1") {
            Clause cls(ClauseType::FOLLOWS, ret, ref);
            expected.setReturnReference(&ret);
            expected.addClause(&cls);

            qp.preprocessQuery(TestQPreprocessor::INPUT_1, actual);
            /*cout << (*actual.getClauses()[0]).getFirstReference()->getValue() << endl;
            cout << (cls.getFirstReference()->getValue()) << endl;
            cout << (*actual.getClauses()[0]).getSecondReference()->getValue() << endl;
            cout << (cls.getSecondReference()->getValue()) << endl;*/

            REQUIRE((actual.getClauses()[0])->equals(cls));
            REQUIRE((actual.getClauses()[0])->equals(*expected.getClauses()[0]));
            REQUIRE((actual.getReturnReference()->equals(*expected.getReturnReference())));
        }
        
        SECTION("test 2") {
            expected.setReturnReference(&ret);
            qp.preprocessQuery(TestQPreprocessor::INPUT_2, actual);

            REQUIRE(actual.getClauses().size() == 0);
            REQUIRE((actual.getReturnReference()->equals(*expected.getReturnReference())));
        }
    }

    SECTION("test query not created due to missing return entity") {
        Query expected;
        Query actual;
        Reference decl(DesignEntityType::STMT, ReferenceType::CONSTANT, "s");
        Reference ret(DesignEntityType::STMT, ReferenceType::SYNONYM, "p");
        expected.setReturnReference(&ret);

        REQUIRE_THROWS(qp.preprocessQuery(TestQPreprocessor::INPUT_3, actual));
        //REQUIRE(actual.getClauses().size() == 0);
    }
}
