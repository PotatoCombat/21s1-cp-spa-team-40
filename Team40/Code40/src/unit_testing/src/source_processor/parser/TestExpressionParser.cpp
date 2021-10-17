#include "TestParserUtils.h"
#include "source_processor/parser/ExpressionParser.h"

TEST_CASE("ExpressionParser: parseExpression - expressions") {
    Statement stmt = TestParserUtils::createAssignStmt(1, "test", "(1+(1/2))+((1*3)+((1-4)+(1%5)))");

    auto parser = ExpressionParser({"(","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear after (");

    parser = ExpressionParser({"(","1","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear after ( and operator must appear before (");

    parser = ExpressionParser({"(","1","(","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear after ( and operator must appear before (");

    parser = ExpressionParser({"(","1","+","(","1","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: invalid variable, constant or operator encountered");

    parser = ExpressionParser({"(","1","+","(","1","/",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear after ( and operator must appear before (");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: invalid variable, constant or operator encountered");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear after ( and operator must appear before (");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: invalid variable, constant or operator encountered");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear after ( and operator must appear before (");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: invalid variable, constant or operator encountered");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before )");
}

TEST_CASE("ExpressionParser: parseExpression - conditions") {
    Statement stmt = TestParserUtils::createIfStmt(1, "((a!=b)&&(!(x==y)))||((c>=x)&&(x==y))");

    auto parser = ExpressionParser({"(","(","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear after (");

    parser = ExpressionParser({"(","(","a","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: invalid variable, constant or operator encountered");

    parser = ExpressionParser({"(","(","a","!=",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","(","a","!=","b",")","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear after ( and operator must appear before (");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: invalid variable, constant or operator encountered");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear after ( and operator must appear before (");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: invalid variable, constant or operator encountered");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before ) and operator must appear after )");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear after ( and operator must appear before (");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: invalid variable, constant or operator encountered");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: factor or ! must appear before )");
}

TEST_CASE("ExpressionParser: parseExpression - brackets") {
    Statement stmt = TestParserUtils::createIfStmt(1, "((a!=b)&&(!(x==y)))||((c>=x)&&(x==y))");

    auto parser = ExpressionParser({"(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","(","a","!=","b","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: ) must appear before logical operator");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: ( must appear after logical operator");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: ( must appear after logical operator");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","c",">=","x",")","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x","&&","(","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: ) must appear before logical operator");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","x","==","y",")",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: ( must appear after logical operator");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y",")"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","(","a","!=","b",")","&&","(","!","(","x","==","y",")",")",")","||","(","(","c",">=","x",")","&&","(","x","==","y"}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    stmt = TestParserUtils::createAssignStmt(1, "test", "(1+(1/2))+((1*3)+((1-4)+(1%5)))");
    
    parser = ExpressionParser({"1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3","+","(","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","1","-","4",")","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4","+","(","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","1","%","5",")",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");

    parser = ExpressionParser({"(","1","+","(","1","/","2",")",")","+","(","(","1","*","3",")","+","(","(","1","-","4",")","+","(","1","%","5",")",}, &stmt);
    REQUIRE_THROWS_WITH(parser.parseExpression(), "invalid expression: brackets do not match");
}
