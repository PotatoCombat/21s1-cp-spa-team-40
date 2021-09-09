//#include "catch.hpp"
//
//#include "query_processor/query_preprocessor/pql_model/clause/SelectClause.h"
//
//struct TestSelectClause {
//    static const string RET;
//    static Declaration createDeclaration();
//    static SuchThatClause createStClause();
//    //static PatternClause createPtClause();
//    static SelectClause createClause();
//};
//
//const string TestSelectClause::RET = "a";
//
//Declaration TestSelectClause::createDeclaration() {
//    return Declaration(TestSelectClause::RET, DesignEntityType::ASSIGN);
//}
//
//SuchThatClause TestSelectClause::createStClause() {
//    Relation rel = Relation(Reference("_"), 
//                            Reference(TestSelectClause::RET), 
//                            RelationType::FOLLOWS);
//    return SuchThatClause(rel);
//}
//
////PatternClause TestSelectClause::createPtClause() {
////    return NULL;
////}
//
//SelectClause TestSelectClause::createClause() {
//    SelectClause cl;
//    cl.setReturnEntity(TestSelectClause::RET);
//    Declaration decl = TestSelectClause::createDeclaration();
//    SuchThatClause stCl = TestSelectClause::createStClause();
//    //PatternClause ptCl = TestSelectClause::createPtClause();
//    cl.addDeclaration(decl);
//    cl.addSuchThatClause(stCl);
//
//    return cl;
//}
//
//TEST_CASE("QP-SelectClause: constructor, set methods") {
//    SelectClause cl = TestSelectClause::createClause();
//
//    REQUIRE(cl.getReturnEntity() == TestSelectClause::RET);
//    REQUIRE(cl.getDeclarations()[0].getSynonym() == TestSelectClause::createDeclaration().getSynonym());
//    REQUIRE(cl.getSuchThatClauses()[0].getRelation().getRelationType() == 
//        TestSelectClause::createStClause().getRelation().getRelationType());
//    REQUIRE(cl.getPatternClauses().empty());
//}
