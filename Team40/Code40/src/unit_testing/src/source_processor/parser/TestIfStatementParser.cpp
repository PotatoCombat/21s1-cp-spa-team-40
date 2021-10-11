#include "TestParserUtils.h"
#include "source_processor/parser/IfStatementParser.h"

vector<Line> ifProgramLines = {Line(0, {"procedure", "sumDigits", "{"}), 
                             Line(1, {"if", "(", "num1", ">", "num2", ")", "then", "{"}), 
                             Line(1, {"}"}), 
                             Line(1, {"else", "{"}), 
                             Line(1, {"}"}), 
                             Line(1, {"}"})};

TEST_CASE("IfStatementParser: parseIfStatement") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = IfStatementParser({"if", "(", "num1", ">", "num2", ")", "then", "{"}, 
                                          INDEX, ifProgramLines).parseIfStatement(INDEX);
    INDEX = 1;
    auto testStmt = TestParserUtils::createIfStmt(INDEX, "num1 > num2");
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
    REQUIRE_THROWS_WITH(*parser.parseIfStatement(INDEX),"invalid expression: invalid variable, constant or operator encountered");

    parser = IfStatementParser({"if", "(", "num1" "num2", ")", "then", "{"}, 
                               INDEX, ifProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseIfStatement(INDEX), "invalid expression");

    parser = IfStatementParser({"if", "(", "num1", ">", ")", "then", "{"}, 
                               INDEX, ifProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseIfStatement(INDEX), "invalid expression");

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
