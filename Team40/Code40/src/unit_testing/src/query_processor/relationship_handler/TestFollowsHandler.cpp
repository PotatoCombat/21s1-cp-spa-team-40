#include "FollowsHandler.h"
#include "../test_util/PKBStub.h"
#include "Result.h"
#include "NamedStatement.h"
#include "SynonymStatement.h"
#include "FollowsRelationship.h"
#include "Entity.h"

#include "catch.hpp"

using namespace std;

PKBStub pkbStub;

TEST_CASE("FollowsHandler: eval - 2 named statements") {
    /*
    NamedStatement stmt1("1");
    NamedStatement stmt2("2");

    FollowsRelationship relationship(&stmt1, &stmt2);
    FollowsHandler handler(&relationship, &pkbStub);
    
    Result result;
    handler.eval(result);
    */
    PKB *pkb = &pkbStub;
    REQUIRE(pkb->follows(1,2));
}
