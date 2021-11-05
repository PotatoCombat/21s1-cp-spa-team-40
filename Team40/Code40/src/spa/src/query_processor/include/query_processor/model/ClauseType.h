#pragma once

enum class ClauseType {
    FOLLOWS,
    FOLLOWS_T,
    PARENT,
    PARENT_T,
    MODIFIES_S,
    MODIFIES_P,
    USES_S,
    USES_P,
    CALLS,
    CALLS_T,
    NEXT,
    NEXT_T,
    NEXTBIP,
    NEXTBIP_T,
    AFFECTS,
    AFFECTS_T,
    WITH,
    PATTERN,
    Count // make sure that this is the last element of this enum for
          // ResultCache to work properly
          // shouldn't be used as a clause type
};
