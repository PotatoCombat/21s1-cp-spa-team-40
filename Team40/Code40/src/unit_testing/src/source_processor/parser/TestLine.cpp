#include "TestParserUtils.h"
#include "source_processor/parser/Line.h"

struct TestLineUtils {
public:
    inline static int index = 1;
    inline static vector<string> contents = {"this", "is", "a", "line"};
};

TEST_CASE("Line: ctor") {
    Line line(TestLineUtils::index, TestLineUtils::contents);
    REQUIRE(line.getIndex() == TestLineUtils::index);

    auto testContents = TestLineUtils::contents;
    auto actualContents = line.getContent();

    REQUIRE(actualContents.size() == testContents.size());

    for (int i = 0; i < testContents.size(); i++) {
        REQUIRE(testContents.at(i) == actualContents.at(i));
    }
}
