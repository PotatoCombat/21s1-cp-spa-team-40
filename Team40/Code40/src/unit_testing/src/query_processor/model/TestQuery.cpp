#include <vector>

#include "query_processor/model/Clause.h"
#include "query_processor/model/Query.h"
#include "query_processor/model/Reference.h"

#include "catch.hpp"

using namespace std;

struct TestQueryHelper {
public:
    void checkQuery(Query *actualQuery, vector<Reference *> expectedReturnRefs,
                    vector<Reference *> expectedRefList,
                    vector<Clause *> expectedClauseList) {
        vector<Reference *> actualReturnRefs =
            actualQuery->getReturnReferences();
        vector<Reference *> actualRefList = actualQuery->getReferences();
        vector<Clause *> actualClauseList = actualQuery->getClauses();

        REQUIRE(actualReturnRefs == expectedReturnRefs);
        REQUIRE(expectedRefList.size() == actualRefList.size());
        REQUIRE(expectedClauseList.size() == actualClauseList.size());

        for (int i = 0; i < expectedRefList.size(); i++) {
            REQUIRE(expectedRefList[i]->equals(*actualRefList[i]));
        }

        for (int i = 0; i < expectedClauseList.size(); i++) {
            REQUIRE(expectedClauseList[i]->equals(*actualClauseList[i]));
        }
    }
};

TestQueryHelper testQueryHelper;

TEST_CASE("Query: query creation - all SYNONYM - adds all to referenceList") {
    Query query;
    Reference returnReference(DesignEntityType::CONSTANT,
                              ReferenceType::SYNONYM, "x",
                              ReferenceAttribute::INTEGER);
    Reference a_syn(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a",
                    ReferenceAttribute::INTEGER);
    Clause clause(ClauseType::USES_S, a_syn, returnReference);
    query.addReturnReference(&returnReference);
    query.addClause(&clause);

    testQueryHelper.checkQuery(&query, vector<Reference *>{&returnReference},
                               vector<Reference *>{&returnReference, &a_syn},
                               vector<Clause *>{&clause});
}

TEST_CASE("Query: query creation - various ReferenceType - only adds SYNONYM") {
    Query query;
    Reference returnReference(DesignEntityType::CONSTANT,
                              ReferenceType::SYNONYM, "x",
                              ReferenceAttribute::INTEGER);
    Reference a_const(DesignEntityType::ASSIGN, ReferenceType::CONSTANT, "1",
                      ReferenceAttribute::INTEGER);
    Reference s_const(DesignEntityType::ASSIGN, ReferenceType::CONSTANT, "3",
                      ReferenceAttribute::INTEGER);
    Reference s_wc(DesignEntityType::STMT, ReferenceType::WILDCARD, "_",
                   ReferenceAttribute::INTEGER);
    Clause clause1(ClauseType::USES_S, a_const, returnReference);
    Clause clause2(ClauseType::FOLLOWS, s_const, s_wc);
    query.addClause(&clause1);
    query.addReturnReference(&returnReference);
    query.addClause(&clause2);

    testQueryHelper.checkQuery(&query, vector<Reference *>{&returnReference},
                               vector<Reference *>{&returnReference},
                               vector<Clause *>{&clause1, &clause2});
}
