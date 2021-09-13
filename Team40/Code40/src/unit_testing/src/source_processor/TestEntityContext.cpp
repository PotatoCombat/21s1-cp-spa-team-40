#include "catch.hpp"
#include "common/model/ConstantValue.h"
#include "source_processor/design_extractor/EntityContext.h"

TEST_CASE("EntityContext: Pushes and pops correctly") {
    const int VALUE = 123;
    ConstantValue cv(VALUE);
    EntityContext<ConstantValue> entityContext;
    entityContext.push(&cv);
    REQUIRE(entityContext.getAllEntities().size() == 1);
    REQUIRE(entityContext.getAllEntities().front()->getValue() == VALUE);
    entityContext.pop(&cv);
    REQUIRE(entityContext.getAllEntities().empty());
}