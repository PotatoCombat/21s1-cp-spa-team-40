#include "query_processor/QueryParser.h"

void QueryParser::clear() {
    this->stParser.clear();
    this->ptParser.clear();
    this->deleteDeclarations();
}

void QueryParser::deleteDeclarations() {
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
Reference *QueryParser::parseReturnSynonym(string ref) {
    ReferenceAttribute attr = getAttrRef(ref);

    for (auto x : declList) {
        if (ref == x->getValue()) {
            return new Reference(x->getDeType(), x->getRefType(), x->getValue(), attr);
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
 * @param patTuple as <stmt, vector<args>>.
 * @return PatternClause object.
 */
PatternClause *QueryParser::parsePatternClause(PatTuple patTuple) {
    return ptParser.parse(patTuple);
}

ReferenceAttribute QueryParser::getAttrRef(string ref) {
    string attr = ParserUtil::getAttribute(ref);
    if (attr.empty()) {
        return ReferenceAttribute::NONE;
    }
    if (attr == "procName" || attr == "varName") {
        return ReferenceAttribute::NAME;
    } else if (attr == "stmt#" || attr == "value") {
        return ReferenceAttribute::INTEGER;
    } else {
        throw ValidityError("Invalid attribute");
    }
}
