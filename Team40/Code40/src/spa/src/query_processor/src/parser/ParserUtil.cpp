#include "query_processor/parser/ParserUtil.h"

ReferenceType ParserUtil::checkRefType(std::string val) {
    if (isWildcard(val)) {
        return ReferenceType::WILDCARD;
    } else if (isInteger(val)) {
        return ReferenceType::CONSTANT;
    } else if (isQuoted(val)) {
        return ReferenceType::CONSTANT;
    }
    throw ValidityError("invalid value");
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

/**
 * Check if string is a valid name. A valid name is one that starts with a
 * LETTER and contains only LETTERs and DIGITs.
 * @param val String to check.
 * @return True if valid, otherwise false.
 */
bool ParserUtil::isValidName(std::string val) {
    if (isalpha(val[0])) {
        auto it = find_if_not(begin(val), end(val), isalnum);
        if (it == val.end()) {
            return true;
        }
    }
    return false;
}

/**
 * Check if string is a attrRef.
 * @param val String to check.
 * @return true if attrRef, otherwise false.
 */
bool ParserUtil::isAttrRef(std::string val) {
    int c = count(val.begin(), val.end(), '.');
    return c == 1;
}

/**
 * Split syn.attr given a attrRef.
 * @param val String to split.
 * @return Pair<syn, attr>.
 */
std::pair<std::string, std::string> ParserUtil::splitAttrRef(std::string val) {
    size_t pos = val.find('.');
    std::string syn = val.substr(0, pos);
    std::string attr = val.substr(pos + 1);
    return make_pair(syn, attr);
}

/**
 * Parse valid attribute.
 * @param deType DesignEntityType enum
 * @param attr The string to parse.
 * @return ReferenceAttribute type.
 * @exception ValidityError if attr is invalid.
 */
ReferenceAttribute ParserUtil::parseValidAttr(DesignEntityType deType,
                                              std::string val) {
    if (val == "procName") {
        if (deType == DesignEntityType::PROCEDURE ||
            deType == DesignEntityType::CALL) {
            return ReferenceAttribute::NAME;
        }
    } else if (val == "varName") {
        if (deType == DesignEntityType::VARIABLE ||
            deType == DesignEntityType::READ ||
            deType == DesignEntityType::PRINT) {
            return ReferenceAttribute::NAME;
        }
    } else if (val == "value") {
        if (deType == DesignEntityType::CONSTANT) {
            return ReferenceAttribute::INTEGER;
        }
    } else if (val == "stmt#") {
        if (!(deType == DesignEntityType::PROCEDURE ||
              deType == DesignEntityType::VARIABLE ||
              deType == DesignEntityType::CONSTANT ||
              deType == DesignEntityType::PROG_LINE)) {
            return ReferenceAttribute::INTEGER;
        }
    }
    throw ValidityError("QP-ERROR: invalid attribute");
}
