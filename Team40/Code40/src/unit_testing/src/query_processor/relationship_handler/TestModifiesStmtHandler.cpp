//#include "../test_util/PKBStub.h"
//#include "../test_util/PKBStub2.h"
//#include "query_processor/Result.h"
//#include "query_processor/model/Clause.h"
//#include "query_processor/model/Reference.h"
//#include "query_processor/relationship_handler/ModifiesStmtHandler.h"
//#include "query_processor/relationship_handler/ClauseHandlerError.h"
//
//#include "catch.hpp"
//
//using namespace std;
//
//struct TestModifiesStmtHandler {
//    static PKBStub pkbStub;
//    //static PKBStub2 pkbStubNoParent;
//};
//
//PKBStub TestModifiesStmtHandler::pkbStub = PKBStub();
////PKBStub2 TestModifiesStmtHandler::pkbStubNoParent = PKBStub2();
//
//TEST_CASE("ModifiessStmtHandler: eval - stmt is WILDCARD - throws ClauseHandlerError") {
//    Result expectedResult;
//    expectedResult.setValid(true);
//
//    Reference stmt1(DesignEntityType::STMT, ReferenceType::WILDCARD, "_");
//    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::WILDCARD, "_");
//    Clause modifiesStmtClause(ClauseType::MODIFIES_S, stmt1, stmt2);
//    ModifiesStmtHandler handler(&modifiesStmtClause, &TestModifiesStmtHandler::pkbStub);
//
//    REQUIRE_THROWS_AS(handler.eval(), ClauseHandlerError);
//}
//
//
//TEST_CASE("ModifiesStmtHandler: eval - CONSTANT CONSTANT - modifies stmt match") {
//    Result expectedResult;
//    expectedResult.setValid(true);
//
//    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "8");
//    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "y");
//    Clause modifiesStmtClause(ClauseType::MODIFIES_S, stmt1, stmt2);
//    ModifiesStmtHandler handler(&modifiesStmtClause, &TestModifiesStmtHandler::pkbStub);
//    Result actualResult = handler.eval();
//
//    REQUIRE(expectedResult.equals(actualResult));
//}
//
//TEST_CASE("ModifiesStmtHandler: eval - CONSTANT CONSTANT - modifies stmt not match") {
//    Result expectedResult;
//    expectedResult.setValid(false);
//
//    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
//    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::CONSTANT, "a");
//    Clause modifiesStmtClause(ClauseType::MODIFIES_S, stmt1, stmt2);
//    ModifiesStmtHandler handler(&modifiesStmtClause, &TestModifiesStmtHandler::pkbStub);
//    Result actualResult = handler.eval();
//
//    REQUIRE(expectedResult.equals(actualResult));
//}
//
//TEST_CASE("ModifiesStmtHandler: eval - CONSTANT WILDCARD - has stmt modifies CONST") {
//    Result expectedResult;
//    expectedResult.setValid(true);
//
//    Reference stmt1(DesignEntityType::STMT, ReferenceType::CONSTANT, "4");
//    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::WILDCARD, "_");
//    Clause modifiesStmtClause(ClauseType::MODIFIES_S, stmt1, stmt2);
//    ModifiesStmtHandler handler(&modifiesStmtClause, &TestModifiesStmtHandler::pkbStub);
//    Result actualResult = handler.eval();
//
//    REQUIRE(expectedResult.equals(actualResult));
//}
//
//TEST_CASE("ModifiesStmtHandler: eval - SYNONYM SYNONYM - returns resultList1, resultList2") {
//    Reference stmt1(DesignEntityType::WHILE, ReferenceType::SYNONYM, "s1");
//    Reference stmt2(DesignEntityType::VARIABLE, ReferenceType::SYNONYM, "s2");
//    Clause ModifiessStmtClause(ClauseType::MODIFIES_S, stmt1, stmt2);
//    ModifiesStmtHandler handler(&ModifiessStmtClause, &TestModifiesStmtHandler::pkbStub);
//    Result actualResult = handler.eval();
//
//    Result expectedResult;
//    expectedResult.setValid(true);
//    vector<string> expectedList1{"4"};
//    vector<string> expectedList2{"i", "x", "y", "z"};
//    expectedResult.setResultList1(&stmt1, expectedList1);
//    expectedResult.setResultList2(&stmt2, expectedList2);
//
//    REQUIRE(expectedResult.equals(actualResult));
//}
