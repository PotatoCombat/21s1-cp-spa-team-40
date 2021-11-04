#include "TestParserUtils.h"
#include "source_processor/parser/WhileStatementParser.h"

vector<Line> whileProgramLines = {Line(0, {"procedure", "sumDigits", "{"}), 
                             Line(1, {"while", "(", "number", ">", "0", ")", "{"}), 
                             Line(2, {"number", "=", "number", "-", "1", ";"}),
                             Line(2, {"}"}), 
                             Line(2, {"}"})};

TEST_CASE("WhileStatementParser: parseEntity") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = WhileStatementParser({"while", "(", "number", ">", "0", ")", "{"}, INDEX, 
                                            whileProgramLines).parseEntity(INDEX);
    INDEX = 1;
    auto testStmt = TestParserUtils::createWhileStmt(INDEX, "number > 0");
    auto testChildStmt = TestParserUtils::createAssignStmt(INDEX + 1, "number", "number - 1");
    testStmt.addThenStmt(&testChildStmt);
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("WhileStatementParser: parseEntity - throws invalid while statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = WhileStatementParser({"while", "", "number", ">", "0", ")", "{"}, INDEX, 
                                       whileProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseEntity(INDEX), "invalid while statement");

    parser = WhileStatementParser({"while", "(", ">", "0", ")", "{"}, INDEX, 
                                  whileProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseEntity(INDEX),"invalid expression: invalid variable, constant or operator encountered");

    parser = WhileStatementParser({"while", "(", "number", "0", ")", "{"}, INDEX, 
                                  whileProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseEntity(INDEX), "invalid expression: invalid variable, constant or operator encountered");

    parser = WhileStatementParser({"while", "(", "number", ">", ")", "{"}, INDEX, 
                                  whileProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseEntity(INDEX), "invalid expression: cannot end with an operator");

    parser = WhileStatementParser({"while", "(", "number", ">", "0", "{"}, INDEX, 
                                  whileProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseEntity(INDEX), "invalid while statement");

    parser = WhileStatementParser({"while", "(", "number", ">", "0", ")"}, INDEX, 
                                  whileProgramLines);
    REQUIRE_THROWS_WITH(*parser.parseEntity(INDEX), "invalid while statement");
}
