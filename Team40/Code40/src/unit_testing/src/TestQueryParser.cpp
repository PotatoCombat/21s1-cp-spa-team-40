#include "catch.hpp"

#include "query_processor/query_preprocessor/QueryParser.h"
#include "query_processor/query_preprocessor/pql_model/relation/FollowsRelation.h"

//TEST_CASE("QP-QueryParser: parseQuery") {
//    QueryParser parser;
//    vector<string> decl{"stmt s", "assign a", "print p1"};
//    string returnEntity = "s";
//    vector<string> stcl{"Follows(s, p1)"};
//    vector<string> ptcl;
//    tuple<string, vector<string>, vector<string>> cl =
//        make_tuple(returnEntity, stcl, ptcl);
//
//    Declaration stmt("s", DesignEntityType::STMT);
//    Declaration assign("a", DesignEntityType::ASSIGN);
//    Declaration print("p1", DesignEntityType::PRINT);
//
//    SuchThatClause suchThatClause(FollowsRelation(Reference("s"), Reference("p1")));
//    SelectClause result = parser.parseQuery(decl, cl);
//    vector<Declaration> resultDecl = result.getDeclarations();
//    vector<SuchThatClause> resultStcl = result.getSuchThatClauses();
//    vector<PatternClause> resultPtcl = result.getPatternClauses();
//    REQUIRE(resultDecl.size() == 3);
//    // REQUIRE(resultDecl[0] == stmt);
//    // REQUIRE(resultDecl[1] == assign);
//    // REQUIRE(resultDecl[2] == print);
//
//    REQUIRE(resultDecl.size() == 1);
//    //REQUIRE(resultStcl[0] == suchThatClause);
//
//    REQUIRE(resultPtcl.size() == 0);
//}
