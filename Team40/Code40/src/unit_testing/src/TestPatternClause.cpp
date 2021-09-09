//#include "catch.hpp"
//
//#include "query_processor/query_preprocessor/pql_model/clause/PatternClause.h"
//
//struct TestPtClause {
//    static const string SYN;
//    static const string REF;
//    static const string EXPR;
//    static PatternClause createClause();
//};
//
//const string TestPtClause::SYN = "a";
//const string TestPtClause::REF = "v";
//const string TestPtClause::EXPR = "_\"x*y\"_";
//
//PatternClause TestPtClause::createClause() {
//    return PatternClause(
//        TestPtClause::SYN, 
//        EntityReference(TestPtClause::REF), 
//        Expression(TestPtClause::EXPR));
//}
//
//TEST_CASE("QP-PatternClause: constructor, get methods") {
//    PatternClause cl = TestPtClause::createClause();
//
//    REQUIRE(cl.getSynonym() == TestPtClause::SYN);
//    REQUIRE(cl.getEntRef().getValue() == TestPtClause::REF);
//    REQUIRE(cl.getExpression().getExpr() == TestPtClause::EXPR);
//}
