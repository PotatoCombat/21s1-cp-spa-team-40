#include "TestParserUtils.h"
#include "source_processor/parser/IfStatementParser.h"

vector<Line> ifProgramLines = {Line(0, {"procedure", "sumDigits", "{"}), 
                             Line(1, {"if", "(", "num1", ">", "num2", ")", "then", "{"}), 
                             Line(2, {"max", "=", "num1", ";"}),
                             Line(2, {"}"}), 
                             Line(2, {"else", "{"}), 
                             Line(3, {"max", "=", "num2", ";"}),
                             Line(3, {"}"}), 
                             Line(3, {"}"})};

TEST_CASE("IfStatementParser: parseIfStatement") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = IfStatementParser({"if", "(", "num1", ">", "num2", ")", "then", "{"}, 
                                          INDEX, ifProgramLines).parseIfStatement(INDEX);
    INDEX = 1;
    auto testStmt = TestParserUtils::createIfStmt(INDEX, "num1 > num2");
    auto testChild1Stmt = TestParserUtils::createAssignStmt(INDEX + 1, "max", "num1");
    TestParserUtils::addThenStmt(testStmt, testChild1Stmt);
    auto testChild2Stmt = TestParserUtils::createAssignStmt(INDEX + 2, "max", "num2");
    TestParserUtils::addElseStmt(testStmt, testChild2Stmt);
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("IfStatementParser: parseIfStatement - throws invalid if statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = IfStatementParser({"if", "num1", ">", "num2", ")", "then", "{"}, 
                                    INDEX, ifProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseIfStatement(INDEX), "invalid if statement");

    parser = IfStatementParser({"if", "(", ">", "num2", ")", "then", "{"}, 
                               INDEX, ifProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseIfStatement(INDEX),"invalid expression: conditions need at least one operator");

    parser = IfStatementParser({"if", "(", "num1" "num2", ")", "then", "{"}, 
                               INDEX, ifProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseIfStatement(INDEX), "invalid expression: conditions need at least one operator");

    parser = IfStatementParser({"if", "(", "num1", ">", ")", "then", "{"}, 
                               INDEX, ifProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseIfStatement(INDEX), "invalid expression: cannot end with an operator");

    parser = IfStatementParser({"if", "(", "num1", ">", "num2" "then", "{"}, 
                               INDEX, ifProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseIfStatement(INDEX), "invalid if statement");

    parser = IfStatementParser({"if", "(", "num1", ">", "num2", ")", "{"}, 
                               INDEX, ifProgramLines); 
    REQUIRE_THROWS_WITH(*parser.parseIfStatement(INDEX), "invalid if statement");

    parser = IfStatementParser({"if", "(", "num1", ">", "num2", ")"}, 
                               INDEX, ifProgramLines); 
    REQUIRE_THROWS_WITH(*parser.parseIfStatement(INDEX),"invalid if statement");
}
