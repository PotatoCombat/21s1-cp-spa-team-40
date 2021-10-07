#include "TestParserUtils.h"
#include "source_processor/parser/IfStatementParser.h"

vector<Line> ifProgramLines = {Line(0, {"procedure", "sumDigits", "{"}), 
                             Line(1, {"if", "(", "num1", ">", "num2", ")", "then", "{"}), 
                             Line(1, {"}"}), 
                             Line(1, {"else", "{"}), 
                             Line(1, {"}"}), 
                             Line(1, {"}"})};

TEST_CASE("IfStatementParser: parseIfStatement") {
    int index = 1;
    auto *actualStmt = IfStatementParser({"if", "(", "num1", ">", "num2", ")", "then", "{"}, 
                                          index, ifProgramLines).parseIfStatement(index);
    index = 1;
    auto testStmt = TestParserUtils::createIfStmt(index, "num1 > num2");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("IfStatementParser: parseIfStatement - throws invalid if statement") {
    int index = 1;
    auto parser = IfStatementParser({"if", "num1", ">", "num2", ")", "then", "{"}, 
                                    index, ifProgramLines);
    REQUIRE_THROWS(*parser.parseIfStatement(index));

    parser = IfStatementParser({"if", "(", ">", "num2", ")", "then", "{"}, 
                               index, ifProgramLines);
    REQUIRE_THROWS(*parser.parseIfStatement(index));

    parser = IfStatementParser({"if", "(", "num1" "num2", ")", "then", "{"}, 
                               index, ifProgramLines);
    REQUIRE_THROWS(*parser.parseIfStatement(index));

    parser = IfStatementParser({"if", "(", "num1", ">", ")", "then", "{"}, 
                               index, ifProgramLines);
    REQUIRE_THROWS(*parser.parseIfStatement(index));

    parser = IfStatementParser({"if", "(", "num1", ">", "num2" "then", "{"}, 
                               index, ifProgramLines);
    REQUIRE_THROWS(*parser.parseIfStatement(index));

    parser = IfStatementParser({"if", "(", "num1", ">", "num2", ")", "{"}, 
                               index, ifProgramLines); 
    REQUIRE_THROWS(*parser.parseIfStatement(index));

    parser = IfStatementParser({"if", "(", "num1", ">", "num2", ")"}, 
                               index, ifProgramLines); 
    REQUIRE_THROWS(*parser.parseIfStatement(index));
}
