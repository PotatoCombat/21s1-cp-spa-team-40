#include "Reference.h"
#include "Relation.h"
#include "Query.h"

#include <vector>

#include "catch.hpp"

using namespace std;

struct TestQueryHelper {
public:
	void checkQuery(Query* actualQuery, Reference* expectedReturnRef,
		vector<Reference*> expectedRefList, vector<Relation*> expectedRelaList) {
        Reference* actualReturnRef = actualQuery->getReturnReference();
		vector<Reference*> actualRefList = actualQuery->getReferences();
        vector<Relation*> actualRelaList = actualQuery->getRelations();

        REQUIRE(actualQuery->getReturnReference()->equals(*expectedReturnRef));
        REQUIRE(expectedRefList.size() == actualRefList.size());
        REQUIRE(expectedRelaList.size() == actualRelaList.size());

		for (int i = 0; i < expectedRefList.size(); i++) {
            REQUIRE(expectedRefList[i]->equals(*actualRefList[i]));
		} 

		for (int i = 0; i < expectedRelaList.size(); i++) {
			REQUIRE(expectedRelaList[i]->equals(*actualRelaList[i]));        
		}
	}
};

TestQueryHelper testQueryHelper;

TEST_CASE("Query: query creation - all SYNONYM - adds all to referenceList") {
	Query query;
    Reference returnReference(DesignEntityType::CONSTANT, ReferenceType::SYNONYM, "x");
	Reference a_syn(DesignEntityType::ASSIGN, ReferenceType::SYNONYM, "a");
    Relation relation(RelationType::USES_S, a_syn, returnReference);
	query.setReturnReference(&returnReference);
    query.addRelation(&relation);

	testQueryHelper.checkQuery(&query, &returnReference,
                               vector<Reference *>{&returnReference, &a_syn},
                               vector<Relation *>{&relation});
}

TEST_CASE("Query: query creation - various RefernceType - only adds SYNONYM") {
    Query query;
	Reference returnReference(DesignEntityType::CONSTANT, ReferenceType::SYNONYM, "x");
	Reference a_const(DesignEntityType::ASSIGN, ReferenceType::CONSTANT, "1");
	Reference s_const(DesignEntityType::ASSIGN, ReferenceType::CONSTANT, "3");
	Reference s_wc(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
    Relation relation1(RelationType::USES_S, a_const, returnReference);
	Relation relation2(RelationType::FOLLOWS, s_const, s_wc);
    query.addRelation(&relation1);
	query.setReturnReference(&returnReference);
    query.addRelation(&relation2);

	testQueryHelper.checkQuery(&query, &returnReference,
                            vector<Reference *>{&returnReference},
                            vector<Relation *>{&relation1, &relation2});
}
