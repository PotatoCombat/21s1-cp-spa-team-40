#include <functional>
#include <vector>

#include "pkb/Iterator.h"

#include "catch.hpp"

using namespace std;

struct TestIterator {
public:
    static vector<int> createItems();

    static bool isEven(int x);

    struct isEqual
    {
    public:
        isEqual(int y);

        bool operator () (int x) const {
            return x == y;
        }
    private:
        int y;
    };
};

vector<int> TestIterator::createItems() {
    return vector<int> {1, 2, 3, 4, 6, 5, 5, 10, 5};
}

bool TestIterator::isEven(int x) {
    return !(x & 1);
}

TestIterator::isEqual::isEqual(int y) : y(y) {  }

TEST_CASE("Iterator - filter") {
    vector<int> in = TestIterator::createItems();

    Iterator<int> iter(in);
    vector<int> actual = iter
            .filter(TestIterator::isEven)
            .filter(TestIterator::isEqual(10))
            .asVector();

    vector<int> testImmutable = TestIterator::createItems();

    REQUIRE(testImmutable.size() == in.size());

    for (int i = 0; i < in.size(); i++)
    {
        REQUIRE(testImmutable.at(i) == in.at(i));
    }

    vector<int> testResult {10};

    REQUIRE(testResult.size() == actual.size());

    for (int i = 0; i < actual.size(); i++)
    {
        REQUIRE(testResult.at(i) == actual.at(i));
    }
}
