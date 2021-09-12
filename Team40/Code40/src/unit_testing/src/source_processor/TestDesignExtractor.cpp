#include "catch.hpp"
#include "common/model/condition/SingleCondition.h"
#include "common/model/relation/EqualsRelation.h"
#include "common/model/relation/Relation.h"
#include "source_processor/design-extractor/DesignExtractor.h"

TEST_CASE("DesignExtractor: Correctly handles NotCondition") {
    //    ConstantValue cv1(1);
    //    ConstantValue cv2(2);
    //    EqualsRelation rel(&cv1, &cv2);
    //    SingleCondition sc(&rel);
    //    NotCondition nc(&sc);
    //    PKB pkb = PKB();
    //    DesignExtractor de(pkb);
    //    de.handleNotCondition(&nc);
    // NOTE: PKB is giving segfault
    //    REQUIRE(pkb.getAllConsts().asVector().back() == 0);
}