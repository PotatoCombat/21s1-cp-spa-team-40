#include "catch.hpp"
#include "common/model/ConstantValue.h"
#include "source_processor/design_extractor/EntityContext.h"

TEST_CASE("EntityContext: Pushes and pops correctly") {
    const string VALUE = "123";
    ConstantValue cv(VALUE);
    EntityContext<ConstantValue> entityContext;
    entityContext.push(&cv);
    REQUIRE(entityContext.getCopy().size() == 1);
    REQUIRE(entityContext.getCopy().front()->getId() == VALUE);
    entityContext.pop(&cv);
    REQUIRE(entityContext.empty());
}