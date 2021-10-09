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

bool ParserUtil::isWildcard(std::string val) { return val == "_"; }

bool ParserUtil::isQuote(std::string val) { return val == "\""; }

bool ParserUtil::isInteger(std::string val) {
    return all_of(val.begin(), val.end(), isdigit);
}

bool ParserUtil::isQuoted(std::string val) {
    int c = count(val.begin(), val.end(), '"');

    if (c != 2) {
        return false;
    }

    size_t pos1 = val.find('"');
    size_t pos2 = val.rfind('"');

    if (pos1 == 0 && pos2 == val.size() - 1) {
        return true;
    }
    return false;
}

bool ParserUtil::isUnderscoredQuoted(std::string val) {
    int c_u = count(val.begin(), val.end(), '_');
    int c_q = count(val.begin(), val.end(), '"');
    if (c_u != 2 || c_q != 2) {
        return false;
    }

    size_t pos1 = val.find('_');
    size_t pos2 = val.rfind('_');

    if (pos1 == 0 && pos2 == val.size() - 1) {
        pos1 = val.find('"');
        pos2 = val.rfind('"');
        if (pos1 == 1 && pos2 == val.size() - 2) {
            return true;
        }
    }
    return false;
}

/**
 * Check if a string is in syn.attr syntax.
 * @param val String to check.
 * @return True if string has attribute, otherwise false.
 */
std::string ParserUtil::getAttribute(std::string val) {
    int c = count(val.begin(), val.end(), '.');
    if (c != 1) {
        return "";
    }

    size_t pos = val.find('.');
    return val.substr(pos + 1);
}
