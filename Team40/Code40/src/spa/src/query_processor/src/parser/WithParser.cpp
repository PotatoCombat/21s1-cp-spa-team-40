#include "query_processor/parser/WithParser.h"

WithParser::WithParser() {
    this->ref1 = "";
    this->ref2 = "";
}

void WithParser::initReferences(vector<Reference *> &declList) {
    this->declList = declList;
}

void WithParser::clear() {
    this->declList.clear();
    this->ref1 = "";
    this->ref2 = "";
}

/**
 * Parse a WithPair into a Clause object.
 * @param withPair Pair of <ref, ref>.
 * @return Clause object of type WITH.
 */
Clause *WithParser::parse(WithPair withPair) {
    this->ref1 = withPair.first;
    this->ref2 = withPair.second;

    // the two refs must be of the same type (both strings or both integers)
    // ref: '"' IDENT '"' | INTEGER | attrRef | synonym
    // synonym must be of type prog_line

    bool isInteger = true;




    return;
}

/**
 * Retrieves synonym in the declaration list if it exists.
 * @param syn The synonym name.
 * @return Reference object if match, otherwise nullptr.
 */
Reference *WithParser::getReferenceIfDeclared(string syn) {
    auto it = find_if(declList.begin(), declList.end(), [&syn](Reference *ref) {
        return ref->getValue() == syn;
    });

    if (it != declList.end()) {
        return *it;
    }
    return nullptr;
}
