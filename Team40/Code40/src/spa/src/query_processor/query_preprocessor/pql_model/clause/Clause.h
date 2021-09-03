#pragma once

enum class ClauseType { UNKNOWN, SUCH_THAT, PATTERN };

class Clause {
public:
    Clause();
    virtual ClauseType getType();
};
