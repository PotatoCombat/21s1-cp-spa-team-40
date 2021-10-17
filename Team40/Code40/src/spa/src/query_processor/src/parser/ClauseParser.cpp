#include "query_processor/parser/ClauseParser.h"

DesignEntityTypeHelper ClauseParser::deHelper = DesignEntityTypeHelper();
ClauseTypeHelper ClauseParser::clsHelper = ClauseTypeHelper();

void ClauseParser::initReferences(vector<Reference *> &declList) {
    this->declList = declList;
}

Clause *ClauseParser::parseSt(ClsTuple clsTuple) {
    throw runtime_error("invalid call");
}

Clause *ClauseParser::parsePt(PatTuple patTuple) {
    throw runtime_error("invalid call");
}

Clause *ClauseParser::parseWt(WithPair withPair) {
    throw runtime_error("invalid call");
}

/**
 * Retrieves synonym in the declaration list if it exists.
 * @param syn The synonym name.
 * @return Reference object if match, otherwise nullptr.
 */
Reference *ClauseParser::getReferenceIfDeclared(string syn) {
    auto it = find_if(declList.begin(), declList.end(), [&syn](Reference *ref) {
        return ref->getValue() == syn;
    });

    if (it != declList.end()) {
        return *it;
    }
    return nullptr;
}
