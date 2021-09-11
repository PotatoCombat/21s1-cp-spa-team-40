#include "Reference.h"
#include "Clause.h"
#include "Query.h"

#include <vector>

#include "catch.hpp"

using namespace std;

struct TestQueryHelper {
public:
	void checkQuery(Query* actualQuery, Reference* expectedReturnRef,
		vector<Reference*> expectedRefList, vector<Clause*> expectedClauseList) {
        Reference* actualReturnRef = actualQuery->getReturnReference();
		vector<Reference*> actualRefList = actualQuery->getReferences();
        vector<Clause*> actualClauseList = actualQuery->getClauses();

        REQUIRE(actualQuery->getReturnReference()->equals(*expectedReturnRef));
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
    Reference returnReference(DesignEntityType::CONSTANT, ReferenceType::SYNONYM, "x");
	Reference a_syn(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
    Clause clause(ClauseType::USES_S, a_syn, returnReference);
	query.setReturnReference(&returnReference);
    query.addClause(&clause);

	testQueryHelper.checkQuery(&query, &returnReference,
                               vector<Reference *>{&returnReference, &a_syn},
                               vector<Clause *>{&clause});
}

TEST_CASE("Query: query creation - various RefernceType - only adds SYNONYM") {
    Query query;
	Reference returnReference(DesignEntityType::CONSTANT, ReferenceType::SYNONYM, "x");
	Reference a_const(DesignEntityType::ASSIGN, ReferenceType::CONSTANT, "1");
	Reference s_const(DesignEntityType::ASSIGN, ReferenceType::CONSTANT, "3");
	Reference s_wc(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Clause clause1(ClauseType::USES_S, a_const, returnReference);
	Clause clause2(ClauseType::FOLLOWS, s_const, s_wc);
    query.addClause(&clause1);
	query.setReturnReference(&returnReference);
    query.addClause(&clause2);

	testQueryHelper.checkQuery(&query, &returnReference,
                            vector<Reference *>{&returnReference},
                            vector<Clause *>{&clause1, &clause2});
}
