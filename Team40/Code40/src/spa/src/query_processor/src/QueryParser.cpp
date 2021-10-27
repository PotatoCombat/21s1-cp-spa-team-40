#include "query_processor/QueryParser.h"

void QueryParser::clear() {
    this->stParser.clear();
    this->ptParser.clear();
    this->wtParser.clear();
    this->clearDeclarations();
}

void QueryParser::clearDeclarations() {
    for (size_t i = 0; i < this->declList.size(); ++i) {
        delete this->declList[i];
    }
    this->declList.clear();
}

/**
 * Parses declared synonyms and store them inside parser.
 * @param declPairs Pairs of declarations as <type, synonym>.
 */
void QueryParser::parseDeclarations(vector<DeclPair> declPairs) {
    for (auto x : declPairs) {
        DesignEntityType deType = deHelper.valueToDesType(x.first);
        ReferenceType refType = ReferenceType::SYNONYM;
        string syn = x.second;
        auto it =
            find_if(declList.begin(), declList.end(),
                    [&syn](Reference *ref) { return ref->getValue() == syn; });

        if (it != declList.end()) {
            throw ValidityError("QP-ERROR: synonym has been declared");
        }
        ReferenceAttribute attr = deHelper.typeToDefaultAttr(deType);
        Reference *ref = new Reference(deType, refType, syn, attr);
        declList.push_back(ref);
    }

    stParser.initReferences(declList);
    ptParser.initReferences(declList);
    wtParser.initReferences(declList);
}

/**
 * Parses return reference by checking if it is in the declaration list.
 * @param ref Return reference to parse.
 * @param &found Whether the reference is found.
 * @return Reference object, otherwise nullptr.
 */
Reference *QueryParser::parseReturnSynonym(string ref) {
    string syn = ref;
    string attrStr = "";

    bool isAttrRef = ParserUtil::isAttrRef(ref);
    if (isAttrRef) {
        pair<string, string> attrRef = ParserUtil::splitAttrRef(ref);
        syn = attrRef.first;
        attrStr = attrRef.second;
    }

    for (auto x : declList) {
        if (syn == x->getValue()) {
            DesignEntityType deType = x->getDeType();
            ReferenceType refType = x->getRefType();
            ReferenceAttribute attr = x->getAttr();
            if (isAttrRef) {
                attr = ParserUtil::parseValidAttr(deType, attrStr);
            }
            if (deType == DesignEntityType::PROG_LINE) {
                deType = DesignEntityType::STMT;
            }
            return new Reference(deType, refType, syn, attr);
        }
    }
    return nullptr;
}

/**
 * Parses a `such that` clause.
 * @param clsTuple Tuple as <type, ref1, ref2>.
 * @return Clause object.
 */
Clause *QueryParser::parseSuchThatClause(ClsTuple clsTuple) {
    return stParser.parse(clsTuple);
}

/**
 * Parses a `pattern` clause.
 * @param patTuple as <stmt, var, vector<token>>.
 * @return Clause object.
 */
Clause *QueryParser::parsePatternClause(PatTuple patTuple) {
    return ptParser.parse(patTuple);
}

/**
 * Parses a `with` clause.
 * @param withPair as <ref, ref>.
 * @return Clause object.
 */
Clause *QueryParser::parseWithClause(WithPair withPair) {
    return wtParser.parse(withPair);
}
