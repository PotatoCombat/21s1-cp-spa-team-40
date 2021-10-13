#include "query_processor/QueryParser.h"

void QueryParser::clear() {
    this->stParser.clear();
    this->ptParser.clear();
    this->clearDeclarations();
}

void QueryParser::clearDeclarations() {
    for (int i = 0; i < this->declList.size(); ++i) {
        delete this->declList[i];
    }
    this->declList.clear();
}

/**
 * Parse declared synonyms and store them inside parser.
 * @param declPairs Pairs of declarations as <type, synonym>.
 */
void QueryParser::parseDeclarations(vector<DeclPair> declPairs) {
    for (auto x : declPairs) {
        DesignEntityType type = deHelper.valueToDesType(x.first);
        string syn = x.second;
        if (!ParserUtil::isValidName(syn)) {
            throw ValidityError("QP-ERROR: invalid name");
        }
        Reference *ref = new Reference(type, ReferenceType::SYNONYM, syn);
        declList.push_back(ref);
    }

    stParser.initReferences(declList);
    ptParser.initReferences(declList);
}

/**
 * Parse return reference by checking if it is in the declaration list.
 * @param ref Return reference to parse.
 * @param &found Whether the reference is found.
 * @return Reference object.
 * @todo Check for syn.attr syntax and set attr for reference object
 * @todo Check for attr validity based on synonym type
 */
Reference *QueryParser::parseReturnSynonyms(string ref) {
    ReferenceAttribute attr = parseValidAttr(ref);

    for (auto x : declList) {
        if (ref == x->getValue()) {
            return new Reference(x->getDeType(), x->getRefType(), ref, attr);
        }
    }
    return nullptr;
}

/**
 * Parse a `such that` clause.
 * @param clsTuple Tuple as <type, ref1, ref2>.
 * @return Clause object.
 */
Clause *QueryParser::parseSuchThatClause(ClsTuple clsTuple) {
    return stParser.parse(clsTuple);
}

/**
 * Parse a `pattern` clause.
 * @param patTuple as <stmt, var, vector<token>>.
 * @return Clause object.
 */
Clause *QueryParser::parsePatternClause(PatTuple patTuple) {
    return ptParser.parse(patTuple);
}

/**
 * Parse valid attribute.
 * @param ref The string to parse.
 * @return Valid attribute.
 * @exception ValidityError if ref/attr is invalid.
 */
ReferenceAttribute QueryParser::parseValidAttr(string ref) {
    string attr = ParserUtil::getAttribute(ref);
    if (attr.empty()) {
        return ReferenceAttribute::DEFAULT;
    }
    if (attr == "procName" || attr == "varName") {
        return ReferenceAttribute::NAME;
    } else if (attr == "stmt#" || attr == "value") {
        return ReferenceAttribute::INTEGER;
    } else {
        throw ValidityError("QP-ERROR: invalid attribute");
    }
}
