#include "query_processor/parser/ParserUtil.h"

ReferenceType ParserUtil::checkRefType(std::string val) {
    if (isWildcard(val)) {
        return ReferenceType::WILDCARD;
    } else if (isInteger(val)) {
        return ReferenceType::CONSTANT;
    } else if (isQuoted(val)) {
        return ReferenceType::CONSTANT;
    }
    throw ValidityError("undeclared synonym");
}

bool ParserUtil::isInteger(std::string val) {
    return all_of(val.begin(), val.end(), isdigit);
}

bool ParserUtil::isQuoted(std::string val) {
    int c = count(val.begin(), val.end(), '"');

    if (c != 2) {
        return false;
    }

    size_t pos1 = val.find_first_of('"');
    size_t pos2 = val.find_last_of('"');

    if (pos1 == 0 && pos2 == val.size() - 1) {
        return true;
    }
    return false;
}

bool ParserUtil::isWildcard(std::string val) { return val == "_"; }
