#pragma once

#include <string>
#include <vector>

#include "query_processor/parser/ClauseParser.h"

class WithParser : public ClauseParser {
public:
    WithParser() : ClauseParser() {}

    void clear();

private:
    Clause *parseWt(WithPair withPair) override;

    Reference *parseReference(string ref);

    Reference *r1 = nullptr;
    Reference *r2 = nullptr;
    string ref1;
    string ref2;
};
