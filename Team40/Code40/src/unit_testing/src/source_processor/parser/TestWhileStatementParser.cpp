#include "TestParserUtils.h"
#include "source_processor/parser/WhileStatementParser.h"

vector<Line> programLines = {Line(0, {"procedure", "sumDigits", "{"}), 
                             Line(1, {"while", "(", "number", ">", "0", ")", "{"}), 
                             Line(1, {"}"}), 
                             Line(1, {"}"})};

TEST_CASE("WhileStatementParser: parseWhileStatement") {
    int index = 1;
    auto *actualStmt = WhileStatementParser({"while", "(", "number", ">", "0", ")", "{"}, index, 
                                            programLines).parseWhileStatement(index);
    index = 1;
    auto testStmt = TestParserUtils::createWhileStmt(index, "number > 0");
    REQUIRE(*actualStmt == testStmt);
    delete actualStmt;
}

TEST_CASE("WhileStatementParser: parseWhileStatement - throws invalid while statement") {
    int index = 1;
    vector<Line> programLines = {};
    auto parser = WhileStatementParser({"while", "number", ">", "0", ")", "{"}, index, 
                                        programLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(index));

    parser = WhileStatementParser({"while", "(", ">", "0", ")", "{"}, index, 
                                            programLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(index));

    parser = WhileStatementParser({"while", "(", "number", "0", ")", "{"}, index, 
                                            programLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(index));

    parser = WhileStatementParser({"while", "(", "number", ">", ")", "{"}, index, 
                                            programLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(index));

    parser = WhileStatementParser({"while", "(", "number", ">", "0", "{"}, index, 
                                            programLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(index));

    parser = WhileStatementParser({"while", "(", "number", ">", "0", ")"}, index, 
                                            programLines);
    REQUIRE_THROWS(*parser.parseWhileStatement(index));
}


