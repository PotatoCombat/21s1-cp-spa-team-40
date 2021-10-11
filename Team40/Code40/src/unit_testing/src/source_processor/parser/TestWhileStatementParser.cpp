#include "TestParserUtils.h"
#include "source_processor/parser/WhileStatementParser.h"

vector<Line> whileProgramLines = {Line(0, {"procedure", "sumDigits", "{"}), 
                             Line(1, {"while", "(", "number", ">", "0", ")", "{"}), 
                             Line(1, {"}"}), 
                             Line(1, {"}"})};

TEST_CASE("WhileStatementParser: parseWhileStatement") {
    int INDEX = TestParserUtils::INDEX;
    auto *actualStmt = WhileStatementParser({"while", "(", "number", ">", "0", ")", "{"}, INDEX, 
                                            whileProgramLines).parseWhileStatement(INDEX);
    INDEX = 1;
    auto testStmt = TestParserUtils::createWhileStmt(INDEX, "number > 0");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("WhileStatementParser: parseWhileStatement - throws invalid while statement") {
    int INDEX = TestParserUtils::INDEX;
    auto parser = WhileStatementParser({"while", "", "number", ">", "0", ")", "{"}, INDEX, 
                                       whileProgramLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(INDEX));

    parser = WhileStatementParser({"while", "(", ">", "0", ")", "{"}, INDEX, 
                                  whileProgramLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(INDEX));

    parser = WhileStatementParser({"while", "(", "number", "0", ")", "{"}, INDEX, 
                                  whileProgramLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(INDEX));

    parser = WhileStatementParser({"while", "(", "number", ">", ")", "{"}, INDEX, 
                                  whileProgramLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(INDEX));

    parser = WhileStatementParser({"while", "(", "number", ">", "0", "{"}, INDEX, 
                                  whileProgramLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(INDEX));

    parser = WhileStatementParser({"while", "(", "number", ">", "0", ")"}, INDEX, 
                                  whileProgramLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(INDEX));
}
